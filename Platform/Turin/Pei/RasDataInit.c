/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  RasDataInit.c
 * @brief Initialize RAS data prior to openSIL execution..
 *
 */

#include <Library/DebugLib.h>
#include <stdint.h>
#include <stddef.h>
#include <Sil-api.h>
#include <xSIM-api.h>
#include <RAS/Common/RasClass-api.h>

/**
 * SetRasData
 *
 * @brief   Set the data in RAS init IP block
 *
 * @details Locate and initialize the IP block for RAS
 *
 * @return  EFI_SUCCESS   IP block successfully initialized
 * @return  EFI_NOT_FOUND IP block structure not found
 *
 */
EFI_STATUS
SetRasData (
  void
  )
{
  RASCLASS_DATA_BLK *RasData;

  RasData = (RASCLASS_DATA_BLK *)SilFindStructure (SilId_RasClass,  0);
  DEBUG ((DEBUG_ERROR, "SIL RAS memory block is found at: 0x%x \n", RasData));
  if (RasData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  // Initialize RasData from Host-FW PCDs
  RasData->RasInputBlock.Temp = 0x1;

  return EFI_SUCCESS;
}

/**
 * OutputRasData
 *
 * @brief   Send RAS output data back to host FW.
 *
 * @details Any data that was changed from RAS and needs to be sent back to
 *          Host-FW should be handled here.
 *
 * @return  EFI_SUCCESS   IP block successfully initialized
 * @return  EFI_NOT_FOUND IP block structure not found
 *
 */
EFI_STATUS
OutputRasData (
  void
  )
{
  RASCLASS_DATA_BLK *RasData;

  RasData = (RASCLASS_DATA_BLK *)SilFindStructure (SilId_RasClass,  0);
  DEBUG ((DEBUG_ERROR, "SIL RAS memory block is found at: 0x%x \n", RasData));
  if (RasData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  return EFI_SUCCESS;
}
