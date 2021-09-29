/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilSmm.h
 * @brief AMD openSIL FW initialization library prototypes
 *
 */

#pragma once

EFI_STATUS
SilFwDataInit (
  EFI_SYSTEM_TABLE    *SystemTable
  );

EFI_STATUS
TranslateStatus (
  SIL_STATUS SilStatus
);
