/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilHob.h
 * @brief Definitions required for Sil Hob data creation/usage.
 *
 */

#pragma once

extern EFI_GUID gPeiOpenSilDataHobGuid;

#pragma pack (push, 1)
typedef struct {
  EFI_PHYSICAL_ADDRESS SilDataPointer;
  UINT32               DataSize;
} SIL_DATA_HOB;
#pragma pack (pop)
