/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilPei.c
 * @brief PEIM created to execute SI init openSIL call.
 *
 */

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemRestore.h>
#include <Ppi/Reset2.h>
#include <Sil-api.h>
#include <CcxClass-api.h>
#include <xSIM-api.h>
#include <Library/SilPei.h>
#include <Ppi/SilPpi.h>
#include <Include/SilHob.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdDirectoryBaseLib.h>

#ifndef SIL_DEADLOOP
#define SIL_DEADLOOP()   \
  {                      \
    volatile size_t __i; \
    __i = 1;             \
    while (__i)          \
      ;                  \
  }
#endif

STATIC PEI_AMD_SIL_INIT_COMPLETE_PPI mSilPeiInitCompletePpi = {
    AMD_SIL_PPI_REVISION};

STATIC EFI_PEI_PPI_DESCRIPTOR mSilPeiInitCompletePpiList =
    {
        (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
        &gAmdSilPeiInitCompletePpiGuid,
        &mSilPeiInitCompletePpi};

EFI_STATUS IPBlockDataBackToHostFW (
  IN CONST EFI_PEI_SERVICES **PeiServices
  );

/**
 * AspValidateMicrocode
 *
 * @brief Validated microcode patch through ASP Mailbox.
 *
 * @param MicroCode       The microcode patch block to validate
 * @param EntrySize       This size of the microcode patch block
 * @param InstanceNumber  This instance number of the entry type (UCODE_PATCH)
 * @return BOOLEAN
 */
BOOLEAN
AspValidateMicrocode (
  IN  CONST EFI_PEI_SERVICES **PeiServices,
  IN   UINT64            MicroCode,
  IN   UINT32            EntrySize,
  IN   UINT8             InstanceNumber
)
{
  MBOX_GENERIC_BUFFER AspMailBoxBuffer;
  RECOVERY_REASON_V2  RecoveryReason;
  UINT32              CurrentPartitionId;
  EFI_PEI_RESET2_PPI *Reset2;

  Reset2 = NULL;

  (*PeiServices)->LocatePpi(PeiServices, &gEfiPeiReset2PpiGuid, 0, NULL, (void **)&Reset2);

  AspMailBoxBuffer.BufferAddrLo = (UINT32)(MicroCode & 0xFFFFFFFF);
  AspMailBoxBuffer.BufferAddrHi = (UINT32)(RShiftU64(MicroCode, 32) & 0xFFFFFFFF);
  AspMailBoxBuffer.BufferSize = EntrySize;
  if (EFI_ERROR(PspMboxBiosCmdValidateBinary (&AspMailBoxBuffer)) && (CheckPspRecoveryFlagV2() == FALSE)) {
    DEBUG ((DEBUG_ERROR, "  ASP Validate Binary Failed.\n"));

    // Set Recovery Reason
    RecoveryReason.EntryType = UCODE_PATCH;
    RecoveryReason.Instance = InstanceNumber & 0xF;
    RecoveryReason.SubProgram = 0;
    RecoveryReason.DirectoryLevel = DIRECTORY_LEVEL_BIOS_L2_ENTRY;
    RecoveryReason.Reserved = 0;
    RecoveryReason.PartitionNumber = 0;
    RecoveryReason.Reserved2 = 0;
    SetRecoveryReason (*((UINT32 *)&RecoveryReason));

    if (!EFI_ERROR(PspMboxBiosGetActiveBootPartitionId (&CurrentPartitionId))) {
      PspMboxBiosSetActiveBootPartitionId (CurrentPartitionId + 1);
    }

    // Trigger Warm Reset
    DEBUG ((DEBUG_INFO, "  Trigger Warm Reset.\n"));
    if (Reset2 != NULL) {
      Reset2->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
    DEBUG ((DEBUG_ERROR, "  Reset Failed. Deadloop...\n"));
    SIL_DEADLOOP ();
  }

  DEBUG ((DEBUG_INFO, "  ASP Validate Binary Successfully.\n"));
  return TRUE;
}

/**
 * GetMicrocodePatchAddress
 *
 * @brief   Get the Microcode Patch Address of the microcode patch matching the ProcessorId input.
 *
 * @details If SignedMicrocodeSupport is enabled, each microcode patch in the microcode patch block is validated
 *          by the ASP before it is checked for matching ProcessorId.
 *
 * @param ProcessorId            The Processor ID of the executing processor
 * @param SignedMicrocodeSupport Flag to enable/disable signed microcode support
 * @param PatchAddress           The patch address of the microcode matching the ProcessorId
 *
 * @return EFI_STATUS
 */
EFI_STATUS
GetMicrocodePatchAddress (
  IN  CONST EFI_PEI_SERVICES **PeiServices,
  IN  UINT16    ProcessorId,
  IN  BOOLEAN   SignedMicrocodeSupport,
  OUT UINT64    *PatchAddress
  )
{
  UINT64                UcodeEntryAddress;
  UINT32                UcodeEntrySize;
  BOOLEAN               UcodeBIOSEntryInfoStatus;
  BOOLEAN               MicrocodeAddrFound;
  UINT8                 InstanceNumber;
  MICROCODE_PATCH_BLOCK *MicroCode;
  EFI_STATUS            Status;

  UcodeEntryAddress        = 0;
  UcodeEntrySize           = 0;
  UcodeBIOSEntryInfoStatus = FALSE;
  InstanceNumber           = 0;
  MicrocodeAddrFound       = FALSE;

  DEBUG ((DEBUG_INFO, "SIL Processor ID : 0x%x \n", ProcessorId));
  // This loop is used find the exact Microcode patch of the executing Processor from the BIOS Directory.
  while (MicrocodeAddrFound == FALSE)
  {
    // Read the patch id from the Microcode block on the BSP
    UcodeBIOSEntryInfoStatus = BIOSEntryInfo(UCODE_PATCH, InstanceNumber, NULL, &UcodeEntryAddress,
                                              &UcodeEntrySize, NULL);

    if (UcodeBIOSEntryInfoStatus == TRUE) {
      if (SignedMicrocodeSupport == TRUE) {
        // If supporting LoadMicrocodePatchV2
        if (AspValidateMicrocode (PeiServices, UcodeEntryAddress, UcodeEntrySize, InstanceNumber)) {
          // If AspValidateMicrocode returns, ASP verification was successful.
          UcodeEntryAddress = (((UINTN)UcodeEntryAddress) + 0x100);
        }
      }

      MicroCode = (MICROCODE_PATCH_BLOCK *)(UINTN)UcodeEntryAddress;
      // compare with running Microprocessor patch ID.
      if (MicroCode->MPB_REVISION.ProcessorRevisionID == ProcessorId) {
        // Update Valid Microcode patch address of the executing processor
        MicrocodeAddrFound = TRUE;
        *PatchAddress = UcodeEntryAddress;
        DEBUG ((DEBUG_INFO, "UcodePatch EntryAddress=%x\n", *PatchAddress));
        Status = EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_INFO, "Microcode Info = %x,%x\n", UcodeEntryAddress,
              MicroCode->MPB_REVISION.ProcessorRevisionID));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "Failed to get the Microcode Block from BiosEntry!\n"));
      Status = EFI_NOT_FOUND;
      break;
    }
    InstanceNumber ++;
  }
  return Status;
}

/**--------------------------------------------------------------------
 * SilTracePoint
 *
 * @brief  Forms output string, translates SIL message level, then calls host debug service
 *
 * @param MsgLevel   The SIL MsgLevel
 * @param Message    The Message to send to debug service
 * @param Function   The name of the calling function
 * @param Line       The line number or the caller
 *
 * @returns void
 **/
void
SilTracePoint (
    size_t      SilMsgLevel,
    const char  *SilPrefix,
    const char  *Message,
    const char  *Function,
    size_t      Line,
    ...)
{
  VA_LIST variadicArgs;
  unsigned short *MsgLvlString;
  uint32_t HostMsgLevel;

  VA_START(variadicArgs, Line);

  switch (SilMsgLevel)
  {
  case SIL_TRACE_ERROR:
    MsgLvlString = L"Error: ";
    HostMsgLevel = DEBUG_ERROR;
    break;
  case SIL_TRACE_WARNING:
    MsgLvlString = L"Warn: ";
    HostMsgLevel = DEBUG_WARN;
    break;
  case SIL_TRACE_ENTRY:
    MsgLvlString = L"Entry ";
    HostMsgLevel = DEBUG_INFO;
    break;
  case SIL_TRACE_EXIT:
    MsgLvlString = L"Exit ";
    HostMsgLevel = DEBUG_INFO;
    break;
  case SIL_TRACE_INFO:
    MsgLvlString = L"Info: ";
    HostMsgLevel = DEBUG_INFO;
    break;
  case SIL_TRACE_VERBOSE:
    MsgLvlString = L"Verbose: ";
    HostMsgLevel = DEBUG_VERBOSE;
    break;
  default:
    MsgLvlString = L"Null: ";
    HostMsgLevel = DEBUG_INFO;
    break;
  }

  // For RAW message level, skip the openSIL prefix
  if (SilMsgLevel != SIL_TRACE_RAW) {
    // Print the openSIL prefix
    DEBUG((HostMsgLevel, SilPrefix));
    // Print function, line, and MsgLvlString
    DEBUG((HostMsgLevel, "%a():%d:%s", Function, Line, MsgLvlString));
  }
  // Print the message with variable arguments
  DebugVPrint(HostMsgLevel, Message, variadicArgs);

  VA_END(variadicArgs);
}

/**--------------------------------------------------------------------
 *  SilHandleReset
 *
 *  @brief  Requests the host to perform the specified reset.
 *
 *  @param  PeiServices Input PeiServices pointer
 *  @param  SilStatus   Input to check if SilStatus contains any reset request
 *
 *  @return void
 */
void SilHandleReset(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    IN SIL_STATUS ResetType)
{
  EFI_PEI_RESET2_PPI *Reset2;
  Reset2 = NULL;

  /*
   * If any reset request was made, handle it.  This statement will compare
   * the input reset request type (ResetType) to the lowest reset request type
   * defined in the SIL_STATUS enum in openSIL.  If the enum changes, it is
   * the responsibility of the user to verify this logic is still valid.
   */
  if (ResetType >= SilResetRequestColdImm)
  {
    (*PeiServices)->LocatePpi(PeiServices, &gEfiPeiReset2PpiGuid, 0, NULL, (void **)&Reset2);

    AmdMemRestoreDiscardCurrentMemContext();
    if (Reset2 != NULL)
    {
      if ((ResetType == SilResetRequestColdDef) || (ResetType == SilResetRequestColdImm))
      {
        DEBUG((DEBUG_ERROR, "%a:%d Performing cold reset\n", __FUNCTION__, __LINE__));
        Reset2->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
        // We should never get this far
        SIL_DEADLOOP();
      }

      if ((ResetType == SilResetRequestWarmDef) || (ResetType == SilResetRequestWarmImm))
      {
        DEBUG((DEBUG_ERROR, "%a:%d Performing warm reset\n", __FUNCTION__, __LINE__));
        Reset2->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
        // We should never get this far
        SIL_DEADLOOP();
      }
    }
  }
}

/**--------------------------------------------------------------------
 * TranslateStatus
 *
 * @brief  Matches EFI_STATUS to a given SIL_STATUS
 *
 * @param SilStatus   Status to be translated to UEF status
 *
 * @returns UEFI status
 **/
EFI_STATUS TranslateStatus(
    SIL_STATUS SilStatus)
{
  switch (SilStatus)
  {
  case SilPass:
    return EFI_SUCCESS;
  case SilDeviceError:
    return EFI_DEVICE_ERROR;
  case SilInvalidParameter:
    return EFI_INVALID_PARAMETER;
  case SilAborted:
    return EFI_ABORTED;
  case SilOutOfResources:
    return EFI_OUT_OF_RESOURCES;
  case SilNotFound:
    return EFI_NOT_FOUND;
  case SilOutOfBounds:
    return EFI_BUFFER_TOO_SMALL;
  default:
    return EFI_UNSUPPORTED;
  }
}

/**
 *  Initialize openSIL services and execute SI
 *
 *  - Query openSIL memory requirements and allocate pool for openSIL
 *  - Execute IP initialization calls to fill IP blocks with the platform data
 *  - Call openSIL SI function
 *  - Install PPI to indicate openSIL execution completion
 *
 *  @param PeiServices  Pointer to PEI services
 *  @param FwInit       Platform FW init function pointer
 *
 *  @return EFI_STATUS Returns EFI_SUCCESS if PPI was successfully installed
 */
EFI_STATUS
SilFwDataInit(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    IN SIL_FWINIT_FUNCTION    FwInit)
{
  EFI_STATUS            Status;
  SIL_STATUS            SilStatus;
  size_t                RequiredMemorySize;
  EFI_PHYSICAL_ADDRESS  SilDataPointer;
  EFI_HOB_GUID_TYPE     *Hob;
  SIL_DATA_HOB          SilDataHob;

  DEBUG((DEBUG_INFO, "SIL PEI entry point..\n"));

  RequiredMemorySize = xSimQueryMemoryRequirements();
  DEBUG((DEBUG_INFO, "openSIL RequiredMemorySize: 0x%x\n", RequiredMemorySize));

  // Allocate PEI memory block and create a HOB that has the pointer to this memory
  Status = PeiServicesAllocatePages(
      EfiBootServicesData,
      EFI_SIZE_TO_PAGES (RequiredMemorySize),
      &SilDataPointer);
  ASSERT_EFI_ERROR(Status);

  // Create openSIL hob with base address of SIL Data as well as the SIL Data size.
  CopyMem(&(SilDataHob.SilDataPointer), &SilDataPointer, sizeof(SilDataPointer));
  SilDataHob.DataSize = (UINT32)RequiredMemorySize;

  DEBUG ((DEBUG_INFO, "SilPei: IP block data pointer is 0x%llx.\n", SilDataPointer));

  Status = PeiServicesCreateHob (
      EFI_HOB_TYPE_GUID_EXTENSION,
      (UINT16)(sizeof(EFI_HOB_GUID_TYPE) + sizeof(SilDataHob)),
      (VOID **)&Hob);
  ASSERT_EFI_ERROR (Status);

  CopyMem(&Hob->Name, &gPeiOpenSilDataHobGuid, sizeof(EFI_GUID));
  Hob++;
  CopyMem (Hob, &SilDataHob, sizeof(SilDataHob));

  DEBUG ((DEBUG_INFO, "openSIL Data HOB @0x%x\n", Hob));
  DEBUG ((DEBUG_INFO, "openSIL Data Block Location @0x%lx\n", (UINT64)SilDataHob.SilDataPointer));
  DEBUG ((DEBUG_INFO, "openSIL Data Block Size 0x%x\n", SilDataHob.DataSize));

  // Let SIL assign the memory to the IPs
  SilStatus = xSimAssignMemoryTp1 ((VOID *)(UINT32)SilDataPointer, RequiredMemorySize);
  if (SilStatus != SilPass)
  {
    DEBUG ((DEBUG_ERROR, "ERROR: Memory assignment status %d.\n", SilStatus));
    return EFI_DEVICE_ERROR;
  }

  if (FwInit != NULL)
  {
    Status = FwInit (PeiServices);
    ASSERT_EFI_ERROR (Status);
  }

  // All IP blocks are configured, pass control to openSIL
  SilStatus = InitializeAMDSiTp1();

  // Send All updated IP block info to HostFW
  Status = IPBlockDataBackToHostFW (PeiServices);
  ASSERT_EFI_ERROR (Status);

  SilHandleReset (PeiServices, SilStatus);

  if (SilStatus != SilPass) {
    DEBUG((DEBUG_ERROR, "ERROR: InitializeAMDSiTp1 Status %d.\n", SilStatus));
    ASSERT (FALSE);
  }

  Status = (**PeiServices).InstallPpi (PeiServices, &mSilPeiInitCompletePpiList);

  return Status;
}
