/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilDataInit.c
 * @brief PEIM created to execute Purico SI openSIL call.
 *
 */

#include <Library/DebugLib.h>
#include <Sil-api.h>
#include <Library/SilPei.h>
#include <Library/xPrfServicesPei.h>

EFI_STATUS SetDfData (VOID);
EFI_STATUS SetConfigRcMgr (VOID);
EFI_STATUS SetCcxData (IN  CONST EFI_PEI_SERVICES **PeiServices);
EFI_STATUS SetFchData (VOID);
EFI_STATUS CcxDataBackToHostFW (VOID);
EFI_STATUS SetMpioData (VOID);
EFI_STATUS SetCxlData (VOID);
EFI_STATUS CxlDataBackToHostFW (VOID);
EFI_STATUS SetNbioData (VOID);
EFI_STATUS FchSyncDataToHostFW (IN CONST EFI_PEI_SERVICES **PeiServices);
EFI_STATUS NbioDataBackToHostFW (VOID);
EFI_STATUS MpioDataBackToHostFW (VOID);

/**
 * PuricoIpBlocksInit
 *
 * Initialize IP blocks with the host FW specific data: PCDs, Setup variables, etc.
 */
EFI_STATUS PuricoIpBlocksInit (
  IN  CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;
  Status = SetDfData ();
  ASSERT_EFI_ERROR (Status);

  Status = SetConfigRcMgr ();
  ASSERT_EFI_ERROR (Status);

  Status = SetFchData ();
  ASSERT_EFI_ERROR (Status);

  Status = SetNbioData ();
  ASSERT_EFI_ERROR (Status);

  Status = SetCcxData (PeiServices);
  ASSERT_EFI_ERROR (Status);

  Status = SetMpioData ();
  ASSERT_EFI_ERROR (Status);

  Status = SetFchData ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
 * IPBlockDataBackToHostFW
 *
 * Send Updated IP blocks Data to host FW specific data: PCDs, Setup variables, etc.
 */
EFI_STATUS IPBlockDataBackToHostFW (
  IN CONST EFI_PEI_SERVICES **PeiServices
)
{
  EFI_STATUS Status;
  Status = FchSyncDataToHostFW (PeiServices);
  ASSERT_EFI_ERROR (Status);

  Status = CcxDataBackToHostFW ();
  ASSERT_EFI_ERROR (Status);

  Status = NbioDataBackToHostFW ();
  ASSERT_EFI_ERROR (Status);

  Status = MpioDataBackToHostFW ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
 *  Purico openSIL FW PEI init driver entry point
 *
 *  @param FileHandle   This file handle
 *  @param PeiServices  Pointer to PEI services
 *
 *  @return EFI_SUCCESS if Ethanol IP block data initialization is successful
 */
EFI_STATUS
EFIAPI
SilPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  Status = SilFwDataInit (PeiServices, PuricoIpBlocksInit);
  ASSERT_EFI_ERROR (Status);

  xPrfServicesPpiInstall();

  return Status;
}
