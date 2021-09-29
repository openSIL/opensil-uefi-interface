/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilDataInit.c
 * @brief DXE driver created to execute Purico SI openSIL call.
 *
 */
#include <Uefi.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Sil-api.h>
#include <Library/SilDxe.h>
#include <CcxClass-api.h>
#include <xSIM-api.h>
#include <xPRF-api.h>
#include <Uefi/UefiSpec.h>
#include <Uefi/UefiBaseType.h>
#include <PiDxe.h>
#include <Library/xPrfServicesDxe.h>
#include <Include/Pi/PiHob.h>

/*
 * Guids produced/consumed by openSIL DXE driver
 */
extern EFI_GUID gPeiOpenSilDataHobGuid;
extern EFI_GUID gEfiPciEnumerationCompleteProtocolGuid;

void CcxAllocateMemoryToRMPTable(void);

/**
 * PciEnumerationCompleteNotification
 *
 * @brief Event handler to be launched after Pci Enumeration Complete Protocol
 * @details This is the openSil 'POST PCI Enumeration' time point (TP2)
 *
 * @return  void
 */
VOID
EFIAPI
PciEnumerationCompleteNotification (
  IN       EFI_EVENT        Event,
  IN       VOID             *Context
  )
{

  DEBUG ((DEBUG_INFO, "openSIL PCI Enumeration Complete callback\n"));
  // Pass control to openSIL for timepoint 2
  InitializeAMDSiTp2 ();

  gBS->CloseEvent(Event);
}

/**
 * SilReadyToBootCallback
 *
 * @brief Event handler to be launched after ready to boot
 * @details This is the openSil 'Pre-OS Boot' time point (TP3)
 *
 * @return  void
 *
 */
VOID
EFIAPI
SilReadyToBootCallback (
  IN       EFI_EVENT        Event,
  IN       VOID             *Context
  )
{

  DEBUG ((DEBUG_INFO, "openSIL ready to boot callback\n"));
  // Pass control to openSIL for timepoint 2
  InitializeAMDSiTp3 ();

  gBS->CloseEvent(Event);
}

/**
 * PuricoIpBlocksInit
 *
 * @brief Initialize IP blocks with the host FW specific data: PCDs, Setup variables, etc.
 */
EFI_STATUS PuricoIpBlocksInit (VOID)
{
  EFI_STATUS Status;
// place Purico IP data init code that depends on Host FW (PCDs, Setup questions, etc.)

  Status = EFI_SUCCESS;

  return Status;
}

/**
 * @brief Purico openSIL FW DXE driver entry point
 *
 * @param ImageHandle   Image handle of DXE driver
 * @param Systemtable   Pointer to UEFI system table
 *
 * @return EFI_SUCCESS
 */
EFI_STATUS
EFIAPI
SilDxeEntryPoint (
  IN       EFI_HANDLE           ImageHandle,
  IN       EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS         Status;
  EFI_EVENT          PciIoEvent;
  EFI_EVENT          BootEvent;
  VOID               *Registration;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "openSIL DXE Driver execution\n"));

  Status = SilFwDataInit (SystemTable, PuricoIpBlocksInit);
  ASSERT_EFI_ERROR (Status);
  // Allocate Secure Nested Paging for Reverse Map Table
  CcxAllocateMemoryToRMPTable();

  // Create PciIo notification to call SillPciIoCallback
  Status = gBS->CreateEvent (
              EVT_NOTIFY_SIGNAL,
              TPL_NOTIFY,
              PciEnumerationCompleteNotification,
              NULL,
              &PciIoEvent
              );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (
              &gEfiPciEnumerationCompleteProtocolGuid,
              PciIoEvent,
              &Registration
              );
  ASSERT_EFI_ERROR (Status);

  // Create ready to boot callback to call SilReadyToBootCallback
  Status = EfiCreateEventReadyToBootEx (
              TPL_CALLBACK,
              SilReadyToBootCallback,
              NULL,
              &BootEvent
              );
  ASSERT_EFI_ERROR (Status);

  // Install xPRF services protocol to provide xPRF interface to UEFI
  Status = xPrfServicesProtocolInstall ();

  return Status;
}
