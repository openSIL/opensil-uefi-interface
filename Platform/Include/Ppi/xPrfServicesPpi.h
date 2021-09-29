/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfServicesPpi.h
 * @brief xPRF services protocol definition
 *
 */

#include <xPRF-api.h>
#include <MEM/MemClass-api.h>
/**
 * xPrf services definitions
 */
typedef EFI_STATUS (*AMD_XPRF_DUMMY_FUNCTION) (VOID);

typedef VOID* (*XPRF_GET_PCIE_TOPOLOGY_STRUCT) (UINT32   *InfoBlockDataSize);
typedef EFI_STATUS (*XPRF_GET_SYSTEM_MEM_MAP) (UINT32*, UINT64*, VOID**);
typedef EFI_STATUS (*XPRF_GET_SMBIOS_MEM_INFO) (VOID*);
typedef EFI_STATUS (*XPRF_GET_MEM_INIT_INFO) (AMD_MEMORY_SUMMARY*);
typedef EFI_STATUS (*XPRF_GET_TRANSLATED_CHANNEL) (UINT8, UINT8*);

/*
 * xPRF Services Protocol definition
 */
typedef struct {
  AMD_XPRF_DUMMY_FUNCTION            Dummy;
  XPRF_GET_PCIE_TOPOLOGY_STRUCT      SilGetNbioTopologyStructure;
  XPRF_GET_SYSTEM_MEM_MAP            SilGetSystemMemoryMap;
  XPRF_GET_SMBIOS_MEM_INFO           SilGetSmbiosMemInfo;
  XPRF_GET_MEM_INIT_INFO             SilGetMemInitInfo;
  XPRF_GET_TRANSLATED_CHANNEL        SilGetTranslatedChannels;
} AMD_OPENSIL_XPRF_SERVICES_PPI;
