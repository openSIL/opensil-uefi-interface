/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfServicesPei.h
 * @brief xPRF services data structures and definitions
 *
 */

#pragma once

#include <MEM/MemClass-api.h>

/**
 * xPrf services Ppi function prototypes
 */
EFI_STATUS
xPrfServicesPpiInstall (VOID);

EFI_STATUS
DummyFunction1 (VOID);

VOID*
SilGetNbiotopologyStructure (
  UINT32   *InfoBlockDataSize
  );


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
);

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
SilGetSmbiosMemInfo(
  VOID *DmiInfo
  );

/**
 * SilGetMemInitInfo
 *
 * @details  This function populate AMD_MEMORY_SUMMARY
 *           structure which is used to report DRAM
 *           info to the Host
 *
 * @param    *MemInitTable   - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval    EFI_SUCCESS             Info extracted successfully.
 * @retval    EFI_INVALID_PARAMETER   Failure.
 **/
EFI_STATUS
SilGetMemInitInfo(
   AMD_MEMORY_SUMMARY *MemInitTable
  );

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
  );
