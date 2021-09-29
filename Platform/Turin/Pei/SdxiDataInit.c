/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  SdxiDataInit.c
 * @brief Initialize Sdxi data prior to openSIL execution.
 *
 */

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Sil-api.h>
#include <xSIM.h>
#include <Sdxi/SdxiClass-api.h>
#include <PiPei.h>

/*
 * SetCmdaData is deprecated.  Pending removal once SDXI support is added to NBIO
 * For more information, see FWDEV-75888
 */

/**
 * SetSdxiData
 *
 * @brief Set the SDXI input defaults
 * @details
 *      Locate the SDXI - the resource initialization IP block
 * @return EFI_SUCCESS or EFI_NOT_FOUND
 */
EFI_STATUS
SetSdxiData (
  void
  )
{
  SDXICLASS_INPUT_BLK    *SdxiData;

  SdxiData = (SDXICLASS_INPUT_BLK *)SilFindStructure (SilId_SdxiClass,  0);
  DEBUG ((DEBUG_ERROR, "SIL SDXI memory block is found at: 0x%x \n", SdxiData));
  if (SdxiData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  SdxiData->AmdFabricSdxi = PcdGetBool (PcdAmdFabricSdxi);

  DEBUG ((DEBUG_INFO, "SIL SDXI PCD AmdFabricSdxi: 0x%x \n", SdxiData->AmdFabricSdxi));

  return EFI_SUCCESS;
}
