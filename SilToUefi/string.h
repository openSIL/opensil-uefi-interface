/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * @file  string.h
 * @brief Standard type definitions and their limits.
 *
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <SilCommon.h>

#define _STRING


NASM_ABI void *memset(void *str, int c, size_t n);
NASM_ABI void *memcpy(void *dest, const void *src, size_t n);

