/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfServicesPei.c
 * @brief xPRF services functions.
 *
 */

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <stddef.h>
#include <stdint.h>
#include <Sil-api.h>
#include <Library/SilPei.h>
#include <Library/xPrfServicesPei.h>
#include <Ppi/xPrfServicesPpi.h>
#include "xPRF-api.h"


extern EFI_GUID gOpenSilxPrfServicePpiGuid;

/*
 * openSIL xPRF services protocol
 */
AMD_OPENSIL_XPRF_SERVICES_PPI   mOpenSilXprfServicePpi = {
  .Dummy                       = DummyFunction1,
  .SilGetNbioTopologyStructure = SilGetNbiotopologyStructure,
  .SilGetSystemMemoryMap       = SilGetSystemMemoryMap,
  .SilGetSmbiosMemInfo         = SilGetSmbiosMemInfo,
  .SilGetMemInitInfo           = SilGetMemInitInfo,
  .SilGetTranslatedChannels    = SilGetTranslatedChannels
};

STATIC EFI_PEI_PPI_DESCRIPTOR mOpenSilXprfServicesPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gOpenSilxPrfServicePpiGuid,
  &mOpenSilXprfServicePpi
};

/**
 * SilGetSystemMemoryMap
 *
 * @details  Get top of memory (Tom2) for the UEFI Host along with
 *           memory map, and number of holes
 *
 * @param    NumberOfHoles      Number of memory holes
 * @param    TopOfSystemMemory  Top of memory address
 * @param    MemHoleDescPtr     Memory descriptor structure
 *
 * @retval   EFI_SUCCESS             Info extracted successfully.
 * @retval   EFI_INVALID_PARAMETER   Failure.
 **/
EFI_STATUS
SilGetSystemMemoryMap (
  UINT32                     *NumberOfHoles,
  UINT64                     *TopOfSystemMemory,
  VOID                       **MemHoleDescPtr
)
{
  SIL_STATUS SilStatus;

  // Call to xPRF service
  if ((NumberOfHoles == NULL) || (TopOfSystemMemory == NULL) || (MemHoleDescPtr == NULL)) {
    DEBUG ((DEBUG_ERROR, "SilGetSystemMemoryMap Status: %r\n", EFI_INVALID_PARAMETER));
    return EFI_INVALID_PARAMETER;
  }

  SilStatus = xPrfGetSystemMemoryMap (NumberOfHoles, TopOfSystemMemory, MemHoleDescPtr);

  // Translate the SIL_STATUS and return it
  return TranslateStatus (SilStatus);
}


/**
 * SilGetSmbiosMemInfo
 *
 * @details This function gets memory related information that
 *          gets used to populate SMBIOS Type 16, Type
 *          17, Type 19 and Type 20.
 *
 * @param   DmiInfo  Point to the DMI records (Type 16, 17, 19, and 20)
 *          Based on Struct SIL_DMI_INFO
 *
 * @retval  EFI_SUCCESS              Info extracted successfully.
 * @retval  EFI_INVALID_PARAMETER    Failure.
 **/
EFI_STATUS
SilGetSmbiosMemInfo (
   VOID *DmiInfo
  )
{
  SIL_STATUS SilStatus;

  // Call to xPRF service
  if (DmiInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "SilGetSmbiosMemInfo Status: %r\n", EFI_INVALID_PARAMETER));
    return EFI_INVALID_PARAMETER;
  }

  SilStatus = xPrfGetSmbiosMemInfo ((SIL_DMI_INFO *)DmiInfo);

  // Translate the SIL_STATUS and return it
  return TranslateStatus (SilStatus);
}

/**
 * SilGetMemInitInfo
 *
 * @details  This function populate AMD_MEMORY_SUMMARY
 *           structure which is used to report DRAM
 *           info to the Host
 *
 * @param    *MemInitTable   - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   EFI_SUCCESS             Info extracted successfully.
 * @retval   EFI_INVALID_PARAMETER   Failure.
 **/
EFI_STATUS
SilGetMemInitInfo (
   AMD_MEMORY_SUMMARY *MemInitTable
  )
{
  SIL_STATUS SilStatus;

  // Call to xPRF service
  if (MemInitTable == NULL) {
    DEBUG ((DEBUG_ERROR, "SilGetMemInitInfo Status: %r\n", EFI_INVALID_PARAMETER));
    return EFI_INVALID_PARAMETER;
  }


  SilStatus = xPrfGetMemInfo (MemInitTable);

  // Translate the SIL_STATUS and return it
  return TranslateStatus (SilStatus);
}

/**
 * SilGetTranslatedChannels
 *
 * @details  This function gets translated channel with requested channel
 *           ID and translate channel ID through the Xlat Table
 *
 * @param    RequestedChannelId   - The requested channel Id
 * @param    *TranslatedChannelId - Pointer to the translated Id
 *
 * @retval   EFI_SUCCESS        Translated Channel extracted successfully.
 * @retval   EFI_NOT_FOUND      Failure.
 **/
EFI_STATUS
SilGetTranslatedChannels (
  UINT8                    RequestedChannelId,
  UINT8                    *TranslatedChannelId
  )
{
  SIL_STATUS    SilStatus;

  SilStatus = xPrfTranslateChannelInfo (RequestedChannelId, TranslatedChannelId);

  return TranslateStatus (SilStatus);
}

/**
 * DummyFunction1
 *
 * @brief   xPRF Ppi wrapper function to test openSIL DummyXprfFunction
 *
 * @details This wrapper function calls xPRF and translates the output.
 *
 * @return  EFI_STATUS The status from openSIL translated into UEFI format.
 */
EFI_STATUS
DummyFunction1 (VOID)
{
  SIL_STATUS SilStatus;

  // Call to xPRF service
  SilStatus = DummyXprfFunction ();
  DEBUG ((DEBUG_INFO, "DummyXprfFunction1 Status: 0x%x\n", SilStatus));

  // Translate the SIL_STATUS and return it
  return TranslateStatus (SilStatus);
}

/**
 * SilGetNbiotopologyStructure
 *
 * @brief   xPrf service wrapper function for openSIL SilFindStructure
 *
 * @details This wrapper function calls xprf function and returns a pointer
 * to the Nbio IP bloack data. It also return the data size.
 *
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  Void* The address from openSIL Instance.
 */
VOID* SilGetNbiotopologyStructure (
  UINT32   *InfoBlockDataSize
  )
{
  return ((VOID *)xPrfGetNbiotopologyStructure (InfoBlockDataSize));
}

/**
 * xPrfServicesProtocolInstall
 *
 * @brief   Installed xPrfServicesProtocol for DXE
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
xPrfServicesPpiInstall (VOID)
{
  EFI_STATUS  Status;

  // Install PPI for NbioBaseServices
  Status = PeiServicesInstallPpi (&mOpenSilXprfServicesPpiList);

  return Status;
}

