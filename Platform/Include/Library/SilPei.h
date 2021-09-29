/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilPei.h
 * @brief AMD openSIL FW initialization library prototypes
 *
 */

#pragma once

/**
 * PcdAmdCcxEnabledFeatures feature bit definitions
 * The definitions were taken from AgesaModulePkg\Include\CcxCommon.h
 */
#define  CCX_FEATURE_CSTATE_LPI             0x00000001
#define  CCX_FEATURE_SIGNED_MICROCODE       0x00000002

typedef EFI_STATUS (*SIL_FWINIT_FUNCTION) (CONST EFI_PEI_SERVICES **PeiServices);

EFI_STATUS SilFwDataInit (CONST EFI_PEI_SERVICES **PeiServices, SIL_FWINIT_FUNCTION FwInitFunction);

EFI_STATUS
GetMicrocodePatchAddress (
  IN  CONST EFI_PEI_SERVICES **PeiServices,
  IN  UINT16    ProcessorId,
  IN  BOOLEAN   SignedMicrocodeSupport,
  OUT UINT64    *PatchAddress
  );

EFI_STATUS
TranslateStatus (
  SIL_STATUS SilStatus
  );

