/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  RcMgrDataInit.c
 * @brief Initialize RC manager data prior to openSIL execution.
 *
 */

#include <Library/DebugLib.h>
#include <RcMgr/DfX/RcManager4-api.h>
#include <Sil-api.h>
#include <xSIM-api.h>
#include <xPRF-api.h>
#include <Library/FabricResourceInitLib.h>

/**
 * SetConfigRcMgr
 *
 * @brief Set the data in resource manager init IP block
 * @details
 *      1. Locate the RcMgrData - the resource initialization IP block
 *      2. Use the found IP block to call FabricResourceInit (AgesaModulePkg/Library/FabricResourceManagerDf3Lib); the IP
 *      block data is updated in the FabricResourceInit call. There are no defaults for RcMgrData, the SetInput() function
 *      of this IP block is NULL.
 * @return EFI_SUCCESS or EFI_DEVICE_ERROR
 */

EFI_STATUS
SetConfigRcMgr (
  void
  )
{
  DFX_RCMGR_INPUT_BLK *RcMgrData;
  EFI_STATUS Status;

  RcMgrData = (DFX_RCMGR_INPUT_BLK *)SilFindStructure(SilId_RcManager,  0);
  if (RcMgrData == NULL) {
    DEBUG ((DEBUG_ERROR, "SIL RC Init memory block was not found.\n"));
    return EFI_NOT_FOUND; // Could not find the IP input block
  }
  DEBUG ((DEBUG_INFO, "SIL RC Init memory block is found blk at: 0x%x \n", RcMgrData));

  RcMgrData->AmdSmee = PcdGetBool(PcdAmdSmee);
  RcMgrData->AmdFabric1TbRemap = PcdGet8(PcdAmdFabric1TbRemap);

  Status = FabricResourceInit (RcMgrData);

  if (EFI_ERROR(Status)) return EFI_DEVICE_ERROR;

  return EFI_SUCCESS;
}
