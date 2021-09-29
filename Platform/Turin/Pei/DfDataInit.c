/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfDataInit.c
 * @brief Initialize Data Fabric input data prior to openSIL execution.
 *
 */

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Sil-api.h>
#include <xSIM.h>
#include <DF/DfClass-api.h>
#include <PiPei.h>

/**
 * SetSdxiData
 *
 * @brief   Set the DF input defaults
 * @details Locate the DF - the resource initialization IP block
 *
 * @return EFI_SUCCESS or EFI_NOT_FOUND
 */
EFI_STATUS
SetDfData (void)
{
  DFCLASS_INPUT_BLK                         *DfData;

  DfData = (DFCLASS_INPUT_BLK *)SilFindStructure (SilId_DfClass,  0);
  DEBUG ((DEBUG_INFO, "SIL DF memory block is found at: 0x%x \n", DfData));
  if (DfData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  DfData->AmdFabricWdtCfg = PcdGet8 (PcdAmdFabricWdtCfg);
  DfData->AmdFabricWdtCntSel = PcdGet8 (PcdAmdFabricWdtCntSel);
  DfData->AmdFabricImmSyncFloodOnFatalErrCtrl = PcdGetBool (PcdAmdFabricImmSyncFloodOnFatalErrCtrl);
  DfData->AmdFabricCcxAsNumaDomain = PcdGetBool(PcdAmdFabricCcxAsNumaDomain);
  DfData->AmdPciExpressBaseAddress = PcdGet64(PcdPciExpressBaseAddress);
  DfData->AmdLongModePageTables = PcdGet32(PcdAmdLongModePageTables);

  return EFI_SUCCESS;
}
