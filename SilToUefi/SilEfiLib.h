/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * @file  SilEfiLib.h
 * @brief SIL to UEFI interface function prototypes
 *
 */

#pragma once

#define memset(s, c, n) SetMem(s, n, c)
