/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CcxDataInit.c
 * @brief Initialize Ccx data prior to openSIL execution..
 *
 */

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Sil-api.h>
#include <CcxClass-api.h>
#include <Library/SilPei.h>

extern EFI_GUID gPeiOpenSilCcxDownCoreDataGuid;

/**
 * SetCcxData
 *
 * @brief Set the data in Ccx init IP block
 * @details
 *      Locate the Ccx - the resource initialization IP block
 * @return EFI_SUCCESS or EFI_NOT_FOUND
 */
EFI_STATUS
SetCcxData (
  IN  CONST EFI_PEI_SERVICES **PeiServices
  )
{
  CCXCLASS_DATA_BLK *CcxData;
  BOOLEAN           SignedMicrocodeSupport;
  UINT64            PatchAddress;

  CcxData = (CCXCLASS_DATA_BLK *)SilFindStructure (SilId_CcxClass,  0);
  DEBUG ((DEBUG_ERROR, "SIL Ccx memory block is found at: 0x%x \n", CcxData));
  if (CcxData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  CcxData->CcxInputBlock.AmdApicMode                    = PcdGet8(PcdAmdApicMode);
  CcxData->CcxInputBlock.AmdIbrsEn                      = PcdGetBool(PcdAmdIbrsEn);
  CcxData->CcxInputBlock.AmdBranchSampling              = PcdGetBool(PcdAmdBranchSampling);
  CcxData->CcxInputBlock.AmdSnpMemCover                 = PcdGet8(PcdAmdSnpMemCover);
  CcxData->CcxInputBlock.AmdVmplEnable                  = PcdGetBool(PcdAmdVmplEnable);
  CcxData->CcxInputBlock.AmdSnpMemSize                  = PcdGet32 (PcdAmdSnpMemSize);
  CcxData->CcxInputBlock.AmdGameMode                    = PcdGetBool(PcdAmdGameMode);
  CcxData->CcxInputBlock.L1BurstPrefetch                = PcdGetBool(PcdAmdL1BurstPrefetch);
  CcxData->CcxInputBlock.AmdL1StreamPrefetcher          = PcdGetBool(PcdAmdL1StreamPrefetcher);
  CcxData->CcxInputBlock.AmdL1StridePrefetcher          = PcdGetBool(PcdAmdL1StridePrefetcher);
  CcxData->CcxInputBlock.AmdL1RegionPrefetcher          = PcdGetBool(PcdAmdL1RegionPrefetcher);
  CcxData->CcxInputBlock.AmdL2StreamPrefetcher          = PcdGetBool(PcdAmdL2StreamPrefetcher);
  CcxData->CcxInputBlock.AmdL2UpDownPrefetcher          = PcdGetBool(PcdAmdL2UpDownPrefetcher);
  CcxData->CcxInputBlock.AmdCpuWdtEn                    = PcdGetBool(PcdAmdCpuWdtEn);
  CcxData->CcxInputBlock.AmdCpuWdtTimeout               = PcdGet16(PcdAmdCpuWdtTimeout);
  if (CcxData->CcxInputBlock.AmdCpuWdtTimeout == 0xFFFF) {
    // If it's 'Auto', set timeout to 2.682s to make sure it's smaller than DF WDT
    CcxData->CcxInputBlock.AmdCpuWdtTimeout             = 0x100;
  }
  CcxData->CcxInputBlock.AmdCpuWdtSeverity              = PcdGet8(PcdAmdCpuWdtSeverity);
  CcxData->CcxInputBlock.AmdCStateMode                  = PcdGet8(PcdAmdCStateMode);
  CcxData->CcxInputBlock.AmdCc6Ctrl                     = PcdGet8(PcdAmdCc6Ctrl);
  CcxData->CcxInputBlock.AmdCStateIoBaseAddress         = PcdGet16(PcdAmdCStateIoBaseAddress);
  CcxData->CcxInputBlock.AmdCpbMode                     = PcdGet8(PcdAmdCpbMode);
  CcxData->CcxInputBlock.AmdHardwarePrefetchMode        = PcdGet8(PcdAmdHardwarePrefetchMode);
  CcxData->CcxInputBlock.AmdSoftwarePrefetchMode        = PcdGet8(PcdAmdSoftwarePrefetchMode);
  CcxData->CcxInputBlock.AmdSmee                        = PcdGetBool(PcdAmdSmee);
  CcxData->CcxInputBlock.AmdReserved                    = PcdGetBool(PcdAmdReserved);
  CcxData->CcxInputBlock.AmdReserved1                   = PcdGet8(PcdAmdReserved1);
  CcxData->CcxInputBlock.AmdReserved2                   = PcdGet8(PcdAmdReserved2);
  CcxData->CcxInputBlock.StreamingStoresCtrl            = PcdGet8(PcdAmdStreamingStoresCtrl);
  CcxData->CcxInputBlock.EnSpecStFill                   = PcdGetBool(PcdAmdEnSpecStFill);
  CcxData->CcxInputBlock.EnableFSRM                     = PcdGetBool(PcdAmdEnableFSRM);
  CcxData->CcxInputBlock.EnableERMS                     = PcdGetBool(PcdAmdEnableERMS);
  CcxData->CcxInputBlock.EnableRMSS                     = PcdGetBool(PcdAmdEnableRMSS);
  CcxData->CcxInputBlock.EnableSvmAVIC                  = PcdGetBool(PcdAmdEnableSvmAVIC);
  CcxData->CcxInputBlock.IbsHardwareEn                  = PcdGetBool(PcdAmdIbsHardwareEn);
  CcxData->CcxInputBlock.EnableAvx512                   = PcdGet8(PcdAmdCcxEnableAvx512);
  CcxData->CcxInputBlock.EnableSvmX2AVIC                = PcdGetBool(PcdAmdEnableSvmX2AVIC);
  CcxData->CcxInputBlock.MonMwaitDis                    = PcdGet8(PcdAmdMonMwaitDis);
  CcxData->CcxInputBlock.AmdFixedMtrr250                = PcdGet64(PcdAmdFixedMtrr250);
  CcxData->CcxInputBlock.AmdFixedMtrr258                = PcdGet64(PcdAmdFixedMtrr258);
  CcxData->CcxInputBlock.AmdFixedMtrr259                = PcdGet64(PcdAmdFixedMtrr259);
  CcxData->CcxInputBlock.AmdFixedMtrr268                = PcdGet64(PcdAmdFixedMtrr268);
  CcxData->CcxInputBlock.AmdFixedMtrr269                = PcdGet64(PcdAmdFixedMtrr269);
  CcxData->CcxInputBlock.AmdFixedMtrr26A                = PcdGet64(PcdAmdFixedMtrr26A);
  CcxData->CcxInputBlock.AmdFixedMtrr26B                = PcdGet64(PcdAmdFixedMtrr26B);
  CcxData->CcxInputBlock.AmdFixedMtrr26C                = PcdGet64(PcdAmdFixedMtrr26C);
  CcxData->CcxInputBlock.AmdFixedMtrr26D                = PcdGet64(PcdAmdFixedMtrr26D);
  CcxData->CcxInputBlock.AmdFixedMtrr26E                = PcdGet64(PcdAmdFixedMtrr26E);
  CcxData->CcxInputBlock.AmdFixedMtrr26F                = PcdGet64(PcdAmdFixedMtrr26F);
  CcxData->CcxInputBlock.IommuSupport                   = PcdGetBool (PcdCfgIommuSupport);
  CcxData->CcxInputBlock.AmdPstatePolicy                = PcdGet8(PcdAmdAgesaPstatePolicy);
  CcxData->CcxInputBlock.AmdSplitRmpTable               = 0;
  CcxData->CcxInputBlock.CpuPauseCntSel_1_0             = PcdGet8(PcdAmdCpuPauseCntSel_1_0);
  CcxData->CcxInputBlock.DisFstStrErmsb                 = PcdGet8(PcdAmdCcxDisFstStrErmsb);
  CcxData->CcxInputBlock.SvmEnable                      = PcdGetBool(PcdAmdSvmEnable);
  CcxData->CcxInputBlock.SvmLock                        = PcdGetBool(PcdAmdSvmLock);
  CcxData->CcxInputBlock.CcxLoadUcodePatch              = PcdGetBool(PcdAmdCcxLoadUcodePatch);


  // Set the microcode patch address in the CCX input block for openSIL to load during CCX execution.
  DEBUG ((DEBUG_ERROR, "PcdAmdCcxEnabledFeatures: 0x%x \n", (PcdGet32 (PcdAmdCcxEnabledFeatures))));
  SignedMicrocodeSupport = (PcdGet32 (PcdAmdCcxEnabledFeatures) & CCX_FEATURE_SIGNED_MICROCODE) ? TRUE : FALSE;
  DEBUG ((DEBUG_ERROR, "SignedMicrocodeSupport: 0x%x \n", SignedMicrocodeSupport));
  GetMicrocodePatchAddress (PeiServices,
                            CcxData->CcxOutputBlock.ProcessorId,
                            SignedMicrocodeSupport,
                            &PatchAddress
                            );
  CcxData->CcxInputBlock.UcodePatchEntryInfo.UcodePatchEntryAddress = PatchAddress;

  return EFI_SUCCESS;
}

/**
 * CcxDataBackToHostFW
 *
 * @brief Send Updated Ccx IP blocks Data to host FW
 * @return EFI_SUCCESS or EFI_NOT_FOUND
 *       EFI_SUCCESS   : Received valid address within the Host allocated memory block
 *                       and succesfully update the PCD.
 *       EFI_NOT_FOUND : Indicates the requested block was not found
 */
EFI_STATUS
CcxDataBackToHostFW (
  void
  )
{
  EFI_STATUS        Status;
  CCXCLASS_DATA_BLK *CcxDataHostFw;

  CcxDataHostFw = (CCXCLASS_DATA_BLK *)SilFindStructure (SilId_CcxClass,  0);
  DEBUG ((DEBUG_INFO, "CcxDataBackToHostFW: SIL CCCX data block at: 0x%x \n", CcxDataHostFw));
  if (CcxDataHostFw == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  if (CcxDataHostFw->CcxOutputBlock.AmdApicMode != 0xFF) {
    // If the AmdApicMode changed from the openSIL default, write it back
    Status = PcdSet8S (PcdAmdApicMode, CcxDataHostFw->CcxOutputBlock.AmdApicMode);
    DEBUG ((DEBUG_INFO, "PcdAmdApicMode update: 0x%x Status:%d\n", PcdGet8 (PcdAmdApicMode), Status));
  }
  Status = PcdSetBoolS (PcdAmdAcpiS3Support, CcxDataHostFw->CcxOutputBlock.AmdAcpiS3Support);
  DEBUG ((DEBUG_INFO, "PcdAmdAcpiS3Support update: 0x%x, Status:%d\n", PcdGetBool (PcdAmdAcpiS3Support), Status));

  return EFI_SUCCESS;
}
