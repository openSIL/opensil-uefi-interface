/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfServicesSmm.h
 * @brief xPRF services data structures and definitions for SMM
 *
 */
#pragma once

#include <Uefi.h>
#include <xPRF-api.h>
#include <Protocol/xPrfServicesSmmProtocol.h>

EFI_STATUS
SilMcaErrorAddrTranslate (
  UINT64                  *SystemMemoryAddress,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress,
  SIL_DIMM_INFO           *DimmInfo,
  SIL_ADDR_DATA           *AddrData
  );

EFI_STATUS
SilTranslateSysAddrToCS (
  UINT64                  *SystemMemoryAddress,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress,
  SIL_DIMM_INFO           *DimmInfo,
  SIL_ADDR_DATA           *AddrData
  );

VOID
SilGetLocalSmiStatus (
  VOID *LocalSmiStatus
  );

VOID
SilCollectMcaErrorInfo (
  VOID *SilRasMcaErrorInfo
  );

EFI_STATUS
SilTranslateSysAddrToDpa (
  UINT64        *SystemMemoryAddress,
  UINT64        *Dpa,
  SIL_ADDR_DATA *AddrData
  );

EFI_STATUS
xPrfServicesSmmProtocolInstall (VOID);
