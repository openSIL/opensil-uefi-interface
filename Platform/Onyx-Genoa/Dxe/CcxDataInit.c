/**
 * @file  CcxDataInit.c
 * @brief The Host will locate a structure within the memory block
 *        that was assigned by an IP block
 *        Allocates the Secure Nested Paging (SNP) for Reverse Map Table (RMP)
 *
 */
/**
 * Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Sil-api.h>
#include <CcxClass-api.h>
#include <Library/AmdPspBaseLibV2.h>
#include <xPRF-api.h>
#include <Uefi/UefiSpec.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>

/**
 * CcxAllocateMemoryToRMPTable
 *
 * @brief   Allocate Secure Nested Paging for Reverse Map Table
 *
 */
void
CcxAllocateMemoryToRMPTable (
 void
 )
{

  EFI_PHYSICAL_ADDRESS    RmpTableBase;
  EFI_STATUS              Status;
  EFI_PHYSICAL_ADDRESS    AlignedTableAddress ;
  CCXCLASS_DATA_BLK       *CcxData;

  RmpTableBase        = 0;
  AlignedTableAddress = 0;

  CcxData = (CCXCLASS_DATA_BLK *)SilFindStructure (SilId_CcxClass,  0);
  if (CcxData == NULL) {
    DEBUG ((DEBUG_ERROR, "OpenSIL DXE Driver Could not found the block memory\n"));
    return; // Could not find the IP input block
  }

  if((CcxData->CcxOutputBlock.AmdIsSnpSupported) &&
     (CcxData->CcxInputBlock.AmdSnpMemCover != 0)) {

    RmpTableBase = (EFI_PHYSICAL_ADDRESS)CcxData->CcxOutputBlock.AmdRmpTableBase;
	// find enough memory for RMP table to fit on MB boundary
    Status = gBS->AllocatePages (AllocateMaxAddress,
                                 EfiReservedMemoryType,
                                 (UINTN)(EFI_SIZE_TO_PAGES (CcxData->CcxOutputBlock.AmdRmpTableSize + SIZE_1MB)),
                                 &RmpTableBase
                                );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[ERROR] Failed to allocate RMP Table.\n"));
      return;
    }

    // align on MB boundary
    AlignedTableAddress = (RmpTableBase + SIZE_1MB - 1) & ~(SIZE_1MB - 1);

    // free pages before reallocating on MB boundary
    gBS->FreePages (RmpTableBase, (UINTN)(EFI_SIZE_TO_PAGES (CcxData->CcxOutputBlock.AmdRmpTableSize + SIZE_1MB)));

    // reserve memory for RMP Table on MB boundary
    Status = gBS->AllocatePages (AllocateAddress,
                                 EfiReservedMemoryType,
                                 (UINTN)(EFI_SIZE_TO_PAGES (CcxData->CcxOutputBlock.AmdRmpTableSize)),
                                 &AlignedTableAddress
                                );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[ERROR] Try to allocate RMP Table at 0x%x but failed.\n",AlignedTableAddress));
      return;
    }

    // zero out RMP table
    gBS->SetMem ((VOID *) AlignedTableAddress,
                 (UINTN)(CcxData->CcxOutputBlock.AmdRmpTableSize),
                 0
                );

    xPrfSetSnpRmp(AlignedTableAddress,CcxData->CcxOutputBlock.AmdRmpTableSize);
  }
  else {
    DEBUG ((DEBUG_ERROR, "AmdIsSnpSupported not supported\n"));
  }
}
