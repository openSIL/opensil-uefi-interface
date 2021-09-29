/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilDxe.h
 * @brief AMD openSIL FW initialization library prototypes
 *
 */

#pragma once

typedef EFI_STATUS (*SIL_FWINIT_FUNCTION) ();

EFI_STATUS
SilFwDataInit (
  EFI_SYSTEM_TABLE    *SystemTable,
  SIL_FWINIT_FUNCTION FwInitFunction
  );

EFI_STATUS
TranslateStatus (
  SIL_STATUS SilStatus
);
