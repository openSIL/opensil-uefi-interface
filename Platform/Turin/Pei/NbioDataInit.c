/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  NbioDataInit.c
 * @brief Initialize NBIO data prior to openSIL execution.
 *
 */

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Sil-api.h>
#include <Nbio/NbioClass-api.h>

/**
 * NbioSetInputBlk
 * @brief Establish NBIO input defaults
 *
 * @retval SIL_STATUS
 */
EFI_STATUS
SetNbioData (
  VOID
  )
{
  NBIOCLASS_DATA_BLOCK *NbioData;

  NbioData = (NBIOCLASS_DATA_BLOCK *)SilFindStructure (SilId_NbioClass,  0);
  DEBUG ((DEBUG_INFO, "SIL NBIO memory block is found at: 0x%x \n", NbioData));
  if (NbioData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  NbioData->NbioConfigData.SevSnpSupport = PcdGetBool (PcdCfgSevSnpSupport);
  NbioData->NbioConfigData.IOHCClkGatingSupport = PcdGetBool (PcdIOHCClkGatingSupport);
  NbioData->NbioConfigData.CfgNbifMgcgClkGating = PcdGetBool (PcdCfgNbifMgcgClkGating);
  NbioData->NbioConfigData.CfgSstunlClkGating = PcdGetBool (PcdCfgSstunlClkGating);
  NbioData->NbioConfigData.CfgSyshubMgcgClkGating = PcdGetBool (PcdCfgSyshubMgcgClkGating);
  NbioData->NbioConfigData.TPHCompleterEnable = PcdGet8 (PcdTPHCompleterEnable);
  NbioData->NbioConfigData.IoApicMMIOAddressReservedEnable = PcdGetBool (PcdCfgIoApicMMIOAddressReservedEnable);
  NbioData->NbioConfigData.IoApicIdPreDefineEn = PcdGetBool (PcdCfgIoApicIdPreDefineEn);
  NbioData->NbioConfigData.IoApicIdBase = PcdGet8 (PcdCfgIoApicIdBase);
  NbioData->NbioConfigData.NbifMgcgHysteresis = PcdGet8 (PcdNbifMgcgHysteresis);
  NbioData->NbioConfigData.SyshubMgcgHysteresis = PcdGet8 (PcdSyshubMgcgHysteresis);
  NbioData->NbioConfigData.IohcNonPCIBarInitDbg = PcdGetBool (PcdCfgIohcNonPCIBarInitDbg);
  NbioData->NbioConfigData.IohcNonPCIBarInitFastReg = PcdGetBool (PcdCfgIohcNonPCIBarInitFastReg);
  NbioData->NbioConfigData.IohcNonPCIBarInitFastRegCtl = PcdGetBool (PcdCfgIohcNonPCIBarInitFastRegCtl);
  NbioData->NbioConfigData.IommuMMIOAddressReservedEnable = PcdGetBool (PcdCfgIommuMMIOAddressReservedEnable);
  NbioData->NbioConfigData.AmdApicMode = PcdGet8 (PcdAmdApicMode);
  NbioData->NbioConfigData.IommuAvicSupport = PcdGetBool (PcdCfgIommuAvicSupport);
  NbioData->NbioConfigData.IommuL2ClockGatingEnable = PcdGetBool (PcdIommuL2ClockGatingEnable);
  NbioData->NbioConfigData.IommuL1ClockGatingEnable = PcdGetBool (PcdIommuL1ClockGatingEnable);
  NbioData->NbioConfigData.IOHCPgEnable = PcdGetBool (PcdIOHCPgEnable);
  NbioData->NbioConfigData.NbioGlobalCgOverride = PcdGet8 (PcdNbioGlobalCgOverride);
  NbioData->NbioConfigData.IommuSupport = PcdGetBool (PcdCfgIommuSupport);
  NbioData->NbioConfigData.CfgACSEnable = PcdGetBool (PcdCfgACSEnable);
  NbioData->NbioConfigData.CfgPCIeLTREnable = PcdGetBool (PcdCfgPCIeLTREnable);
  NbioData->NbioConfigData.CfgPcieAriSupport = PcdGetBool (PcdCfgPcieAriSupport);
  NbioData->NbioConfigData.AmdMaskDpcCapability = PcdGetBool (PcdAmdMaskDpcCapability);
  NbioData->NbioConfigData.PcieEcrcEnablement = PcdGetBool (PcdPcieEcrcEnablement);
  NbioData->NbioConfigData.CfgAutoSpeedChangeEnable = PcdGet8 (PcdCfgAutoSpeedChangeEnable);
  NbioData->NbioConfigData.CfgRxMarginPersistenceMode = PcdGet8 (PcdCfgRxMarginPersistenceMode);
  NbioData->NbioConfigData.CfgSriovEnDev0F1 = PcdGetBool (PcdCfgSriovEnDev0F1);
  NbioData->NbioConfigData.CfgAriEnDev0F1 = PcdGetBool (PcdCfgAriEnDev0F1);
  NbioData->NbioConfigData.CfgAerEnDev0F1 = PcdGetBool (PcdCfgAerEnDev0F1);
  NbioData->NbioConfigData.CfgAcsEnDev0F1 = PcdGetBool (PcdCfgAcsEnDev0F1);
  NbioData->NbioConfigData.CfgAtsEnDev0F1 = PcdGetBool (PcdCfgAtsEnDev0F1);
  NbioData->NbioConfigData.CfgPasidEnDev0F1 = PcdGetBool (PcdCfgPasidEnDev0F1);
  NbioData->NbioConfigData.CfgPwrEnDev0F1 = PcdGetBool (PcdCfgPwrEnDev0F1);
  NbioData->NbioConfigData.CfgRtrEnDev0F1 = PcdGetBool (PcdCfgRtrEnDev0F1);
  NbioData->NbioConfigData.CfgPriEnDev0F1 = PcdGetBool (PcdCfgPriEnDev0F1);
  NbioData->NbioConfigData.AtcEnable = PcdGetBool (PcdAtcEnable);
  NbioData->NbioConfigData.AcsEnRccDev0 = PcdGetBool (PcdAcsEnRccDev0);
  NbioData->NbioConfigData.AerEnRccDev0 = PcdGetBool (PcdAerEnRccDev0);
  NbioData->NbioConfigData.AcsSourceValStrap5 = PcdGetBool (PcdAcsSourceValStrap5);
  NbioData->NbioConfigData.AcsTranslationalBlockingStrap5 = PcdGetBool (PcdAcsTranslationalBlockingStrap5);
  NbioData->NbioConfigData.AcsP2pReqStrap5 = PcdGetBool (PcdAcsP2pReqStrap5);
  NbioData->NbioConfigData.AcsP2pCompStrap5 = PcdGetBool (PcdAcsP2pCompStrap5);
  NbioData->NbioConfigData.AcsUpstreamFwdStrap5 = PcdGetBool (PcdAcsUpstreamFwdStrap5);
  NbioData->NbioConfigData.AcsP2PEgressStrap5 = PcdGetBool (PcdAcsP2PEgressStrap5);
  NbioData->NbioConfigData.AcsDirectTranslatedStrap5 = PcdGetBool (PcdAcsDirectTranslatedStrap5);
  NbioData->NbioConfigData.AcsSsidEnStrap5 = PcdGetBool (PcdAcsSsidEnStrap5);
  NbioData->NbioConfigData.DlfEnStrap1 = PcdGetBool (PcdDlfEnStrap1);
  NbioData->NbioConfigData.Phy16gtStrap1 = PcdGetBool (PcdPhy16gtStrap1);
  NbioData->NbioConfigData.MarginEnStrap1 = PcdGetBool (PcdMarginEnStrap1);
  NbioData->NbioConfigData.PriEnPageReq = PcdGetBool (PcdPriEnPageReq);
  NbioData->NbioConfigData.PriResetPageReq = PcdGetBool (PcdPriResetPageReq);
  NbioData->NbioConfigData.AcsSourceVal = PcdGetBool (PcdAcsSourceVal);
  NbioData->NbioConfigData.AcsTranslationalBlocking = PcdGetBool (PcdAcsTranslationalBlocking);
  NbioData->NbioConfigData.AcsP2pReq = PcdGetBool (PcdAcsP2pReq);
  NbioData->NbioConfigData.AcsP2pComp = PcdGetBool (PcdAcsP2pComp);
  NbioData->NbioConfigData.AcsUpstreamFwd = PcdGetBool (PcdAcsUpstreamFwd);
  NbioData->NbioConfigData.AcsP2PEgress = PcdGetBool (PcdAcsP2PEgress);
  NbioData->NbioConfigData.RccDev0E2EPrefix = PcdGetBool (PcdRccDev0E2EPrefix);
  NbioData->NbioConfigData.RccDev0ExtendedFmtSupported = PcdGetBool (PcdRccDev0ExtendedFmtSupported);
  NbioData->NbioConfigData.PcieSpeedControl = PcdGet8 (PcdAmdPcieSpeedControl);
  NbioData->NbioConfigData.AdvertiseEqToHighRateSupport = PcdGetBool (PcdAmdAdvertiseEqToHighRateSupport);
  NbioData->NbioConfigData.FabricSdxi = PcdGetBool (PcdAmdFabricSdxi);
  NbioData->NbioConfigData.AmdGnbNbifDdrInitEn = PcdGet8 (PcdAmdGnbNbifDdrInitEn);
  NbioData->NbioConfigData.NbifDev0F1AtomicRequestEn = PcdGetBool(PcdNbifDev0F1AtomicRequestEn);
  NbioData->NbioConfigData.AmdHotPlugSettleTime = PcdGet8(PcdAmdHotPlugSettleTime);
  NbioData->NbioConfigData.AmdHotPlugPDSettle = PcdGetBool(PcdAmdHotPlugPDSettle);
  NbioData->NbioConfigData.AmdPresenceDetectSelectMode = PcdGet8(PcdAmdPresenceDetectSelectMode);
  NbioData->NbioConfigData.FchAcpiDeviceInvisibeMapEx = PcdGet16(PcdFchAcpiDeviceInvisibeMapEx);
  NbioData->NbioConfigData.AmdAllowCompliance = PcdGet16(PcdAmdAllowCompliance);
  NbioData->NbioConfigData.AmdAllowComplianceForHpPort = PcdGetBool(PcdAmdAllowComplianceForHpPort);
  NbioData->NbioConfigData.CfgIommuL2AtsCntlEn = PcdGetBool(PcdCfgIommuL2AtsCntlEn);
  NbioData->NbioConfigData.AtomicRoutingEnStrap5 = PcdGetBool(PcdAtomicRoutingEnStrap5);
  NbioData->NbioConfigData.MappingDramIntoHtHole = PcdGetBool(PcdMappingDramIntoHtHole);
  NbioData->NbioConfigData.CfgIommuSocket0Nbio0Enable = PcdGetBool(PcdCfgIommuSocket0Nbio0Enable);
  NbioData->NbioConfigData.CfgIommuSocket0Nbio1Enable = PcdGetBool(PcdCfgIommuSocket0Nbio1Enable);
  NbioData->NbioConfigData.CfgIommuSocket0Nbio2Enable = PcdGetBool(PcdCfgIommuSocket0Nbio2Enable);
  NbioData->NbioConfigData.CfgIommuSocket0Nbio3Enable = PcdGetBool(PcdCfgIommuSocket0Nbio3Enable);
  NbioData->NbioConfigData.CfgIommuSocket1Nbio0Enable = PcdGetBool(PcdCfgIommuSocket1Nbio0Enable);
  NbioData->NbioConfigData.CfgIommuSocket1Nbio1Enable = PcdGetBool(PcdCfgIommuSocket1Nbio1Enable);
  NbioData->NbioConfigData.CfgIommuSocket1Nbio2Enable = PcdGetBool(PcdCfgIommuSocket1Nbio2Enable);
  NbioData->NbioConfigData.CfgIommuSocket1Nbio3Enable = PcdGetBool(PcdCfgIommuSocket1Nbio3Enable);
  NbioData->NbioConfigData.AmdCxlOnAllPorts = PcdGetBool (PcdAmdCxlOnAllPorts);

  return EFI_SUCCESS;
}

/**
 * NbioDataBackToHostFW
 *
 * @brief Send Updated NBIO IP block's data to host FW
 * @return EFI_SUCCESS or EFI_NOT_FOUND
 *       EFI_SUCCESS   : Received valid address within the Host allocated memory block
 *                       and succesfully update the PCD.
 *       EFI_NOT_FOUND : Indicates the requested block was not found
 */
EFI_STATUS
NbioDataBackToHostFW (
  void
  )
{
  EFI_STATUS status;
  NBIOCLASS_DATA_BLOCK *NbioDataHostFw;

  NbioDataHostFw = (NBIOCLASS_DATA_BLOCK *)SilFindStructure (SilId_NbioClass,  0);
  DEBUG ((DEBUG_ERROR, "NbioDataBackToHostFW SIL NBIO memory block at: 0x%x \n", NbioDataHostFw));
  if (NbioDataHostFw == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  status = PcdSetBoolS(PcdCfgSstunlClkGating, NbioDataHostFw->NbioConfigData.CfgSstunlClkGating);
  DEBUG ((DEBUG_ERROR, "PcdCfgSstunlClkGating update: %d\n", status));
  status = PcdSetBoolS(PcdIOHCClkGatingSupport, NbioDataHostFw->NbioConfigData.IOHCClkGatingSupport);
  DEBUG ((DEBUG_ERROR, "PcdIOHCClkGatingSupport update: %d\n", status));
  status = PcdSetBoolS(PcdIOHCPgEnable, NbioDataHostFw->NbioConfigData.IOHCPgEnable);
  DEBUG ((DEBUG_ERROR, "PcdIOHCPgEnable update: %d\n", status));
  status = PcdSetBoolS(PcdCfgNbifMgcgClkGating, NbioDataHostFw->NbioConfigData.CfgNbifMgcgClkGating);
  DEBUG ((DEBUG_ERROR, "PcdCfgNbifMgcgClkGating update: %d\n", status));
  status = PcdSetBoolS(PcdCfgSyshubMgcgClkGating, NbioDataHostFw->NbioConfigData.CfgSyshubMgcgClkGating);
  DEBUG ((DEBUG_ERROR, "PcdCfgSyshubMgcgClkGating update: %d\n", status));
  status = PcdSet8S(PcdNbifMgcgHysteresis, NbioDataHostFw->NbioConfigData.NbifMgcgHysteresis);
  DEBUG ((DEBUG_ERROR, "PcdNbifMgcgHysteresis update: %d\n", status));
  status = PcdSet8S(PcdSyshubMgcgHysteresis, NbioDataHostFw->NbioConfigData.SyshubMgcgHysteresis);
  DEBUG ((DEBUG_ERROR, "PcdSyshubMgcgHysteresis update: %d\n", status));
  status = PcdSet8S(PcdAmdHotPlugSettleTime, NbioDataHostFw->NbioConfigData.AmdHotPlugSettleTime);
  DEBUG ((DEBUG_ERROR, "PcdAmdHotPlugSettleTime update: %d\n", status));
  status = PcdSetBoolS(PcdAmdHotPlugPDSettle, NbioDataHostFw->NbioConfigData.AmdHotPlugPDSettle);
  DEBUG ((DEBUG_ERROR, "PcdAmdHotPlugPDSettle update: %d\n", status));
  status = PcdSet8S(PcdAmdPresenceDetectSelectMode, NbioDataHostFw->NbioConfigData.AmdPresenceDetectSelectMode);
  DEBUG ((DEBUG_ERROR, "PcdAmdPresenceDetectSelectMode update: %d\n", status));
  status = PcdSet16S(PcdFchAcpiDeviceInvisibeMapEx, NbioDataHostFw->NbioConfigData.FchAcpiDeviceInvisibeMapEx);
  DEBUG ((DEBUG_ERROR, "PcdFchAcpiDeviceInvisibeMapEx update: %d\n", status));
  status = PcdSetBoolS(PcdAmdAllowComplianceForHpPort, NbioDataHostFw->NbioConfigData.AmdAllowComplianceForHpPort);
  DEBUG ((DEBUG_ERROR, "PcdAmdAllowComplianceForHpPort update: %d\n", status));
  status = PcdSet8S(PcdAmdGnbNbifDdrInitEn, NbioDataHostFw->NbioConfigData.AmdGnbNbifDdrInitEn);
  DEBUG ((DEBUG_ERROR, "PcdAmdGnbNbifDdrInitEn update: %d\n", status));

  return EFI_SUCCESS;
}
