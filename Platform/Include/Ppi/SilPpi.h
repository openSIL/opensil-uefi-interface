/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilPpi.h
 * @brief AMD openSIL PEI Initialization Complete PPI prototype
 *
 */

#pragma once

typedef struct _PEI_AMD_SIL_INIT_COMPLETE_PPI {
  UINTN    Revision;               ///< Revision Number
} PEI_AMD_SIL_INIT_COMPLETE_PPI;

// Current SilToUefi PPI revision
#define AMD_SIL_PPI_REVISION   0x01

extern EFI_GUID gAmdSilPeiInitCompletePpiGuid;
