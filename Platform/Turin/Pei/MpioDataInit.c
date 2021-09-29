/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  MpioDataInit.c
 * @brief Initialize Mpio data prior to openSIL execution.
 *
 */

#ifndef CONFIG_SOC_F1AM00
#define CONFIG_SOC_F1AM00 1
#endif

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Sil-api.h>
#include <Mpio/MpioClass-api.h>
#include <PiPei.h>
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieComplexPpi.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <string.h>

/**
 * SetMpioData
 *
 * @brief Set the MPIO input defaults
 * @details
 *      Locate the MPIO - the resource initialization IP block
 * @return EFI_SUCCESS or EFI_DEVICE_ERROR
 */
EFI_STATUS
SetMpioData (
  void
  )
{
  EFI_STATUS                    Status;
  MPIOCLASS_INPUT_BLK *MpioData;
  PEI_AMD_NBIO_PCIE_COMPLEX_PPI *NbioPcieComplexPpi;
  DXIO_COMPLEX_DESCRIPTOR       *PcieTopologyData;
  void                          *Source;
  void                          *Destination;
  CONST EFI_PEI_SERVICES        **PeiServices;

  MpioData = (MPIOCLASS_INPUT_BLK *)SilFindStructure (SilId_MpioClass,  0);
  DEBUG ((DEBUG_ERROR, "SIL MPIO memory block is found at: 0x%x \n", MpioData));
  if (MpioData == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  MpioData->CfgDxioClockGating                         = PcdGetBool (PcdCfgDxioClockGating);
  MpioData->PcieDxioTimingControlEnable                = PcdGetBool (PcdPcieDxioTimingControlEnable);
  MpioData->PCIELinkReceiverDetectionPolling           = PcdGet32 (PcdPCIELinkReceiverDetectionPolling);
  MpioData->PCIELinkResetToTrainingTime                = PcdGet32 (PcdPCIELinkResetToTrainingTime);
  MpioData->PCIELinkL0Polling                          = PcdGet32 (PcdPCIELinkL0Polling);
  MpioData->PCIeExactMatchEnable                       = PcdGetBool (PcdPCIeExactMatchEnable);
  MpioData->DxioPhyValid                               = PcdGet8 (PcdDxioPhyValid);
  MpioData->DxioPhyProgramming                         = PcdGet8 (PcdDxioPhyProgramming);
  MpioData->CfgSkipPspMessage                          = PcdGet8 (PcdCfgSkipPspMessage);
  MpioData->DxioSaveRestoreModes                       = PcdGet8 (PcdDxioSaveRestoreModes);
  MpioData->AmdAllowCompliance                         = PcdGet16 (PcdAmdAllowCompliance);
  MpioData->SrisEnableMode                             = PcdGet8 (PcdSrisEnableMode);
  MpioData->SrisSkipInterval                           = PcdGet8 (PcdSrisSkipInterval);
  MpioData->SrisSkpIntervalSel                         = PcdGet8 (PcdSrisSkpIntervalSel);
  MpioData->SrisCfgType                                = PcdGet8 (PcdSrisCfgType);
  MpioData->SrisAutoDetectMode                         = PcdGet8 (PcdSrisAutoDetectMode);
  MpioData->SrisAutodetectFactor                       = PcdGet8 (PcdSrisAutodetectFactor);
  MpioData->SrisLowerSkpOsGenSup                       = PcdGet8 (PcdSrisLowerSkpOsGenSup);
  MpioData->SrisLowerSkpOsRcvSup                       = PcdGet8( PcdSrisLowerSkpOsRcvSup);
  MpioData->AmdCxlOnAllPorts                           = PcdGetBool (PcdAmdCxlOnAllPorts);
  MpioData->CxlCorrectableErrorLogging                 = PcdGetBool (PcdCxlCorrectableErrorLogging);
  MpioData->CxlUnCorrectableErrorLogging               = PcdGetBool (PcdCxlUnCorrectableErrorLogging);
  MpioData->CfgAEREnable                               = PcdGetBool (PcdCfgAEREnable);
  MpioData->CfgRcvErrEnable                            = PcdGetBool( PcdCfgRcvErrEnable);
  MpioData->EarlyBmcLinkTraining                       = PcdGetBool (PcdEarlyBmcLinkTraining);
  MpioData->EarlyBmcLinkSocket                         = PcdGet8 (PcdEarlyBmcLinkSocket);
  MpioData->EarlyBmcLinkLaneNum                        = PcdGet8 (PcdEarlyBmcLinkLaneNum);
  MpioData->EarlyBmcLinkDie                            = PcdGet8 (PcdEarlyBmcLinkDie);
  MpioData->SurpriseDownFeature                        = PcdGetBool (PcdSurpriseDownFeature);
  MpioData->LcMultAutoSpdChgOnLastRateEnable           = PcdGetBool (PcdLcMultAutoSpdChgOnLastRateEnable);
  MpioData->AmdRxMarginEnabled                         = PcdGetBool (PcdAmdRxMarginEnabled);
  MpioData->CfgPcieCVTestWA                            = PcdGet8 (PcdCfgPcieCVTestWA);
  MpioData->CfgPcieAriSupport                          = PcdGetBool (PcdCfgPcieAriSupport);
  MpioData->CfgNbioCTOtoSC                             = PcdGetBool (PcdCfgNbioCTOtoSC);
  MpioData->CfgNbioCTOIgnoreError                      = PcdGetBool (PcdCfgNbioCTOIgnoreError);
  MpioData->CfgNbioSsid                                = PcdGet32 (PcdCfgNbioSsid);
  MpioData->CfgIommuSsid                               = PcdGet32 (PcdCfgIommuSsid);
  MpioData->CfgPspccpSsid                              = PcdGet32 (PcdCfgPspccpSsid);
  MpioData->CfgNtbccpSsid                              = PcdGet32 (PcdCfgNtbccpSsid);
  MpioData->CfgNbifF0Ssid                              = PcdGet32 (PcdCfgNbifF0Ssid);
  MpioData->CfgNtbSsid                                 = PcdGet32 (PcdCfgNtbSsid);
  MpioData->AmdPcieSubsystemDeviceID                   = PcdGet16 (PcdAmdPcieSubsystemDeviceID);
  MpioData->AmdPcieSubsystemVendorID                   = PcdGet16 (PcdAmdPcieSubsystemVendorID);
  MpioData->GppAtomicOps                               = PcdGet8 (PcdGppAtomicOps);
  MpioData->GfxAtomicOps                               = PcdGet8 (PcdGfxAtomicOps);
  MpioData->AmdNbioReportEdbErrors                     = PcdGetBool (PcdAmdNbioReportEdbErrors);
  MpioData->OpnSpare                                   = PcdGet32 (PcdOpnSpare);
  MpioData->MPIOAncDataSupport                         = PcdGetBool (PcdMPIOAncDataSupport);
  MpioData->AfterResetDelay                            = PcdGet32 (PcdAfterResetDelay);
  MpioData->CfgEarlyLink                               = PcdGetBool (PcdCfgEarlyLink);
  MpioData->AmdCfgExposeUnusedPciePorts                = PcdGet8 (PcdAmdCfgExposeUnusedPciePorts);
  MpioData->CfgForcePcieGenSpeed                       = PcdGet8 (PcdCfgForcePcieGenSpeed);
  MpioData->CfgSataPhyTuning                           = PcdGet8 (PcdCfgSataPhyTuning);
  MpioData->PcieLinkComplianceModeAllPorts             = PcdGetBool (PcdPcieLinkComplianceModeAllPorts);
  MpioData->AmdMCTPEnable                              = PcdGetBool (PcdAmdMCTPEnable);
  MpioData->SbrBrokenLaneAvoidanceSup                  = PcdGetBool (PcdSbrBrokenLaneAvoidanceSup);
  MpioData->AutoFullMarginSup                          = PcdGetBool (PcdAutoFullMarginSup);
  // A getter and setter, both are needed for this PCD.
  MpioData->AmdPciePresetMask8GtAllPort                = 0xFFFFFFFF; //PcdGet32 (PcdAmdPciePresetMask8GtAllPort);
  // A getter and setter, both are needed for this PCD.
  MpioData->AmdPciePresetMask16GtAllPort               = 0xFFFFFFFF; //PcdGet32 (PcdAmdPciePresetMask16GtAllPort);
  // A getter and setter, both are needed for this PCD.
  MpioData->AmdPciePresetMask32GtAllPort               = 0xFFFFFFFF; //PcdGet32 (PcdAmdPciePresetMask32GtAllPort);
  MpioData->PcieLinkAspmAllPort                        = PcdGet8 (PcdPcieLinkAspmAllPort);
  MpioData->AmdMCTPMasterSeg                           = PcdGet8(PcdAmdMCTPMasterSeg);
  MpioData->AmdMCTPMasterID                            = PcdGet16(PcdAmdMCTPMasterID);
  MpioData->CfgHotplugUMBSupport                       = PcdGetBool(PcdCfgHotplugUMBSupport);
  MpioData->SyncHeaderByPass                           = PcdGetBool (PcdSyncHeaderByPass);
  MpioData->CxlTempGen5AdvertAltPtcl                   = PcdGetBool (PcdCxlTempGen5AdvertAltPtcl);
  MpioData->AmdPresenceDetectSelectMode                = PcdGet8(PcdAmdPresenceDetectSelectMode);
  MpioData->AmdHotPlugNvmeDefaultMaxPayload            = PcdGet8(PcdAmdHotPlugNVMEDefaultMaxPayload);
  MpioData->LinkDisableAtPowerOffDelay                 = PcdGet8(PcdLinkDisableAtPowerOffDelay);
  MpioData->Enable2SpcGen4                             = PcdGetBool(PcdEnable2SpcGen4);
  MpioData->Enable2SpcGen5                             = PcdGetBool(PcdEnable2SpcGen5);
  MpioData->CfgPcieLoopbackMode                        = PcdGetBool(PcdCfgPcieLoopbackMode);
  MpioData->PcieNonPcieCompliantTrainingFailureSupport = PcdGetBool(PcdPcieNonPcieCompliantTrainingFailureSupport);
  MpioData->CfgDxioTxFIFORdPtrOffset                   = PcdGet8(PcdCfgDxioTxFIFORdPtrOffset);
  MpioData->AmdFabricSdxi                              = PcdGetBool(PcdAmdFabricSdxi);
  MpioData->DataObjectExchange                         = PcdGetBool(PcdDataObjectExchange);
  MpioData->AmdHotPlugForceSFIStrap                    = PcdGetBool(PcdAmdHotPlugForceSFIStrap);
  MpioData->PcieGen3LaneEqUsTxPreset                   = PcdGet8(PcdPcieGen3LaneEqUsTxPreset);
  MpioData->PcieGen3LaneEqDsTxPreset                   = PcdGet8(PcdPcieGen3LaneEqDsTxPreset);
  MpioData->PcieLaneEqPresetMask8GtConfig              = PcdGet8(PcdPcieLaneEqPresetMask8GtConfig);
  MpioData->PcieLaneEqPresetMask8Gt                    = PcdGet32(PcdPcieLaneEqPresetMask8Gt);
  MpioData->PcieGen4LaneEqUsTxPreset                   = PcdGet8(PcdPcieGen4LaneEqUsTxPreset);
  MpioData->PcieGen4LaneEqDsTxPreset                   = PcdGet8(PcdPcieGen4LaneEqDsTxPreset);
  MpioData->PcieLaneEqPresetMask16GtConfig             = PcdGet8(PcdPcieLaneEqPresetMask16GtConfig);
  MpioData->PcieLaneEqPresetMask16Gt                   = PcdGet32(PcdPcieLaneEqPresetMask16Gt);
  MpioData->PcieGen5LaneEqUsTxPreset                   = PcdGet8(PcdPcieGen5LaneEqUsTxPreset);
  MpioData->PcieGen5LaneEqDsTxPreset                   = PcdGet8(PcdPcieGen5LaneEqDsTxPreset);
  MpioData->PcieLaneEqPresetMask32GtConfig             = PcdGet8(PcdPcieLaneEqPresetMask32GtConfig);
  MpioData->PcieLaneEqPresetMask32Gt                   = PcdGet32(PcdPcieLaneEqPresetMask32Gt);
  MpioData->Master7bitSteeringTag                      = PcdGetBool(PcdMaster7bitSteeringTag);
  MpioData->AmdCXlEarlyLinkTraining                    = PcdGet8(PcdAmdCXlEarlyLinkTraining);
  MpioData->AmdHotPlugPDSettle                         = PcdGetBool(PcdAmdHotPlugPDSettle);
  MpioData->AmdAllowComplianceForHpPort                = PcdGetBool(PcdAmdAllowComplianceForHpPort);
  MpioData->TargetPcieGenSpeed                         = PcdGet8(PcdTargetPcieGenSpeed);
  MpioData->AmdDlfCapEnV2                              = PcdGet8(PcdAmdDlfCapEnV2);
  MpioData->AmdDlfExEnV2                               = PcdGet8(PcdAmdDlfExEnV2);
  MpioData->CxlQosNormalLimit                          = PcdGet16 (PcdCxlQosNormalLimit);
  MpioData->CxlQosHighLimit                            = PcdGet16 (PcdCxlQosHighLimit);
  MpioData->CxlQosTimerLimit                           = PcdGet16 (PcdCxlQosTimerLimit);
  MpioData->CxlQosSchedGap                             = PcdGet16 (PcdCxlQosSchedGap);
  MpioData->CxlQosVariableGap                          = PcdGetBool (PcdCxlQosVariableGap);
  MpioData->CxlQosRdspIncMode                          = PcdGetBool (PcdCxlQosRdspIncMode);
  MpioData->CxlQosTimerDecNum                          = PcdGet8 (PcdCxlQosTimerDecNum);
  MpioData->CxlQosRdspIncNum                           = PcdGet8 (PcdCxlQosRdspIncNum);
  MpioData->CxlQosWrrspIncNum                          = PcdGet8 (PcdCxlQosWrrspIncNum);
  MpioData->CfgSevSnpSupport                           = PcdGetBool (PcdCfgSevSnpSupport);
  MpioData->CfgSevTioSupport                           = PcdGetBool (PcdCfgSevTioSupport);
  MpioData->PcieIdeCapSup                              = PcdGetBool (PcdPcieIdeCapSup);
  MpioData->SafeRecoveryBER                            = PcdGetBool (PcdsafeRecoveryBER);
  MpioData->PeriodicCal                                = PcdGetBool (PcdPeriodicCal);

  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgDxioClockGating: 0x%x \n", MpioData->CfgDxioClockGating));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieDxioTimingControlEnable: 0x%x \n", MpioData->PcieDxioTimingControlEnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PCIELinkReceiverDetectionPolling: 0x%x \n", MpioData->PCIELinkReceiverDetectionPolling));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PCIELinkResetToTrainingTime: 0x%x \n", MpioData->PCIELinkResetToTrainingTime));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PCIELinkL0Polling: 0x%x \n", MpioData->PCIELinkL0Polling));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PCIeExactMatchEnable: 0x%x \n", MpioData->PCIeExactMatchEnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD DxioPhyValid: 0x%x \n", MpioData->DxioPhyValid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD DxioPhyProgramming: 0x%x \n", MpioData->DxioPhyProgramming));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgSkipPspMessage: 0x%x \n", MpioData->CfgSkipPspMessage));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD DxioSaveRestoreModes: 0x%x \n", MpioData->DxioSaveRestoreModes));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdAllowCompliance: 0x%x \n", MpioData->AmdAllowCompliance));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdHotPlugHandlingMode: 0x%x \n", MpioData->AmdHotPlugHandlingMode));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisEnableMode: 0x%x \n", MpioData->SrisEnableMode));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisSkipInterval: 0x%x \n", MpioData->SrisSkipInterval));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisSkpIntervalSel: 0x%x \n", MpioData->SrisSkpIntervalSel));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisCfgType: 0x%x \n", MpioData->SrisCfgType));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisAutoDetectMode: 0x%x \n", MpioData->SrisAutoDetectMode));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisAutodetectFactor: 0x%x \n", MpioData->SrisAutodetectFactor));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisLowerSkpOsGenSup: 0x%x \n", MpioData->SrisLowerSkpOsGenSup));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SrisLowerSkpOsRcvSup: 0x%x \n", MpioData->SrisLowerSkpOsRcvSup));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdCxlOnAllPorts: 0x%x \n", MpioData->AmdCxlOnAllPorts));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlCorrectableErrorLogging: 0x%x \n", MpioData->CxlCorrectableErrorLogging));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlUnCorrectableErrorLogging: 0x%x \n", MpioData->CxlUnCorrectableErrorLogging));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgAEREnable: 0x%x \n", MpioData->CfgAEREnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgMcCapEnable: 0x%x \n", MpioData->CfgMcCapEnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgRcvErrEnable: 0x%x \n", MpioData->CfgRcvErrEnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD EarlyBmcLinkTraining: 0x%x \n", MpioData->EarlyBmcLinkTraining));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD EarlyBmcLinkSocket: 0x%x \n", MpioData->EarlyBmcLinkSocket));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD EarlyBmcLinkLaneNum: 0x%x \n", MpioData->EarlyBmcLinkLaneNum));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SurpriseDownFeature: 0x%x \n", MpioData->SurpriseDownFeature));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD LcMultAutoSpdChgOnLastRateEnable: 0x%x \n", MpioData->LcMultAutoSpdChgOnLastRateEnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdRxMarginEnabled: 0x%x \n", MpioData->AmdRxMarginEnabled));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgPcieCVTestWA: 0x%x \n", MpioData->CfgPcieCVTestWA));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgPcieAriSupport: 0x%x \n", MpioData->CfgPcieAriSupport));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgNbioCTOtoSC: 0x%x \n", MpioData->CfgNbioCTOtoSC));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgNbioCTOIgnoreError: 0x%x \n", MpioData->CfgNbioCTOIgnoreError));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgNbioSsid: 0x%x \n", MpioData->CfgNbioSsid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgIommuSsid: 0x%x \n", MpioData->CfgIommuSsid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgPspccpSsid: 0x%x \n", MpioData->CfgPspccpSsid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgNtbccpSsid: 0x%x \n", MpioData->CfgNtbccpSsid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgNbifF0Ssid: 0x%x \n", MpioData->CfgNbifF0Ssid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgNtbSsid: 0x%x \n", MpioData->CfgNtbSsid));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdPcieSubsystemDeviceID: 0x%x \n", MpioData->AmdPcieSubsystemDeviceID));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdPcieSubsystemVendorID: 0x%x \n", MpioData->AmdPcieSubsystemVendorID));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD GppAtomicOps: 0x%x \n", MpioData->GppAtomicOps));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD GfxAtomicOps: 0x%x \n", MpioData->GfxAtomicOps));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdNbioReportEdbErrors: 0x%x \n", MpioData->AmdNbioReportEdbErrors));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD OpnSpare: 0x%x \n", MpioData->OpnSpare));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD MPIOAncDataSupport: 0x%x \n", MpioData->MPIOAncDataSupport));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AfterResetDelay: 0x%x \n", MpioData->AfterResetDelay));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgEarlyLink: 0x%x \n", MpioData->CfgEarlyLink));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdCfgExposeUnusedPciePorts: 0x%x \n", MpioData->AmdCfgExposeUnusedPciePorts));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgForcePcieGenSpeed: 0x%x \n", MpioData->CfgForcePcieGenSpeed));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgSataPhyTuning: 0x%x \n", MpioData->CfgSataPhyTuning));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieLinkComplianceModeAllPorts: 0x%x \n", MpioData->PcieLinkComplianceModeAllPorts));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdMCTPEnable: 0x%x \n", MpioData->AmdMCTPEnable));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SbrBrokenLaneAvoidanceSup: 0x%x \n", MpioData->SbrBrokenLaneAvoidanceSup));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AutoFullMarginSup: 0x%x \n", MpioData->AutoFullMarginSup));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdPciePresetMask8GtAllPort: 0x%x \n", MpioData->AmdPciePresetMask8GtAllPort));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdPciePresetMask16GtAllPort: 0x%x \n", MpioData->AmdPciePresetMask16GtAllPort));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdPciePresetMask32GtAllPort: 0x%x \n", MpioData->AmdPciePresetMask32GtAllPort));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieLinkAspmAllPort: 0x%x \n", MpioData->PcieLinkAspmAllPort));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdMCTPMasterID: 0x%x \n", MpioData->AmdMCTPMasterID));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgHotplugUMBSupport: 0x%x \n", MpioData->CfgHotplugUMBSupport));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SyncHeaderByPass: 0x%x \n", MpioData->SyncHeaderByPass));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlTempGen5AdvertAltPtcl: 0x%x \n", MpioData->CxlTempGen5AdvertAltPtcl));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdPresenceDetectSelectMode: 0x%x \n", MpioData->AmdPresenceDetectSelectMode));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdHotPlugNvmeDefaultMaxPayload: 0x%x \n", MpioData->AmdHotPlugNvmeDefaultMaxPayload));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdLinkDisableAtPowerOffDelay: 0x%x \n", MpioData->LinkDisableAtPowerOffDelay));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdEnable2SpcGen4: 0x%x \n", MpioData->Enable2SpcGen4));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdEnable2SpcGen5: 0x%x \n", MpioData->Enable2SpcGen5));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdCfgPcieLoopbackMode: 0x%x \n", MpioData->CfgPcieLoopbackMode));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdPcieNonPcieCompliantTrainingFailureSupport: 0x%x \n", MpioData->PcieNonPcieCompliantTrainingFailureSupport));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdCfgDxioTxFIFORdPtrOffset: 0x%x \n", MpioData->CfgDxioTxFIFORdPtrOffset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdAmdFabricSdxi: 0x%x \n", MpioData->AmdFabricSdxi));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdDataObjectExchange: 0x%x \n", MpioData->DataObjectExchange));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdAmdHotPlugForceSFIStrap: 0x%x \n", MpioData->AmdHotPlugForceSFIStrap));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdPcieGen3LaneEqUsTxPreset: 0x%x \n", MpioData->PcieGen3LaneEqUsTxPreset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdPcieGen3LaneEqDsTxPreset: 0x%x \n", MpioData->PcieGen3LaneEqDsTxPreset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdPcieLaneEqPresetMask8GtConfig: 0x%x \n", MpioData->PcieLaneEqPresetMask8GtConfig));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcdPcieLaneEqPresetMask8Gt: 0x%x \n", MpioData->PcieLaneEqPresetMask8Gt));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieGen4LaneEqUsTxPreset: 0x%x \n", MpioData->PcieGen4LaneEqUsTxPreset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieGen4LaneEqDsTxPreset: 0x%x \n", MpioData->PcieGen4LaneEqDsTxPreset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieLaneEqPresetMask16GtConfig: 0x%x \n", MpioData->PcieLaneEqPresetMask16GtConfig));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieLaneEqPresetMask16Gt: 0x%x \n", MpioData->PcieLaneEqPresetMask16Gt));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieGen5LaneEqUsTxPreset: 0x%x \n", MpioData->PcieGen5LaneEqUsTxPreset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieGen5LaneEqDsTxPreset: 0x%x \n", MpioData->PcieGen5LaneEqDsTxPreset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieLaneEqPresetMask32GtConfig: 0x%x \n", MpioData->PcieLaneEqPresetMask32GtConfig));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieLaneEqPresetMask32Gt: 0x%x \n", MpioData->PcieLaneEqPresetMask32Gt));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD Master7bitSteeringTag: 0x%x \n", MpioData->Master7bitSteeringTag));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdCXlEarlyLinkTraining: 0x%x \n", MpioData->PcieLaneEqPresetMask32Gt));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdHotPlugPDSettle: 0x%x \n", MpioData->AmdHotPlugPDSettle));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdHotplugPortReset: 0x%x \n", MpioData->AmdHotplugPortReset));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdAllowComplianceForHpPort: 0x%x \n", MpioData->AmdAllowComplianceForHpPort));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD TargetPcieGenSpeed: 0x%x \n", MpioData->TargetPcieGenSpeed));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdDlfCapEnV2: 0x%x \n", MpioData->AmdDlfCapEnV2));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD AmdDlfExEnV2: 0x%x \n", MpioData->AmdDlfExEnV2));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosNormalLimit: 0x%x \n", MpioData->CxlQosNormalLimit));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosHighLimit: 0x%x \n", MpioData->CxlQosHighLimit));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosTimerLimit: 0x%x \n", MpioData->CxlQosTimerLimit));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosSchedGap: 0x%x \n", MpioData->CxlQosSchedGap));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosVariableGap: 0x%x \n", MpioData->CxlQosVariableGap));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosRdspIncMode: 0x%x \n", MpioData->CxlQosRdspIncMode));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosTimerDecNum: 0x%x \n", MpioData->CxlQosTimerDecNum));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosRdspIncNum: 0x%x \n", MpioData->CxlQosRdspIncNum));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CxlQosWrrspIncNum: 0x%x \n", MpioData->CxlQosWrrspIncNum));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgSevSnpSupport: 0x%x \n", MpioData->CfgSevSnpSupport));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD CfgSevTioSupport: 0x%x \n", MpioData->CfgSevTioSupport));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PcieIdeCapSup: 0x%x \n", MpioData->PcieIdeCapSup));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD SafeRecoveryBER: 0x%x \n", MpioData->SafeRecoveryBER));
  DEBUG ((DEBUG_INFO, "SIL MPIO PCD PeriodicCal: 0x%x \n", MpioData->PeriodicCal));


  PeiServices = GetPeiServicesTablePointer ();
  Status = (*PeiServices)->LocatePpi ((CONST EFI_PEI_SERVICES **)PeiServices,
              &gAmdNbioPcieComplexPpiGuid, 0, NULL, (VOID **)&NbioPcieComplexPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gAmdNbioPcieComplexPpiGuid NOT installed!!\n"));
    assert (false);
  }
  NbioPcieComplexPpi->PcieGetComplex (NbioPcieComplexPpi, &PcieTopologyData);
  Source = (void *) ((uint8_t *)PcieTopologyData - sizeof (MPIO_COMPLEX_DESCRIPTOR));
  Destination = &MpioData->PcieTopologyData;
  do {
    Source = (void *) ((uint8_t *) Source + sizeof (MPIO_COMPLEX_DESCRIPTOR));
    memcpy (Destination, Source, sizeof (MPIO_COMPLEX_DESCRIPTOR));
    Destination = (void *) ((uint8_t *) Destination + sizeof (MPIO_COMPLEX_DESCRIPTOR));
  } while ((((MPIO_COMPLEX_DESCRIPTOR *)Source)->Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );
  Source = (void *) ((uint8_t*)MpioData->PcieTopologyData.PlatformData[0].PciePortList - sizeof (MPIO_PORT_DESCRIPTOR));
  MpioData->PcieTopologyData.PlatformData[0].PciePortList = (MPIO_PORT_DESCRIPTOR *) Destination;
  do {
    Source = (void *) ((uint8_t *)Source + sizeof (MPIO_PORT_DESCRIPTOR));
    if (((MPIO_PORT_DESCRIPTOR *)Source)->EngineData.EngineType != MpioUnusedEngine) {
      memcpy (Destination, Source, sizeof (MPIO_PORT_DESCRIPTOR));
      Destination = (void *)((uint8_t *) Destination + sizeof (MPIO_PORT_DESCRIPTOR));
    }
  } while ((((MPIO_PORT_DESCRIPTOR *)Source)->Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );
  if ((MpioData->PcieTopologyData.PlatformData[0].Flags & DESCRIPTOR_TERMINATE_LIST) == 0 ) {
    Source = (void *) ((uint8_t*)MpioData->PcieTopologyData.PlatformData[1].PciePortList - sizeof (MPIO_PORT_DESCRIPTOR));
    MpioData->PcieTopologyData.PlatformData[1].PciePortList = (MPIO_PORT_DESCRIPTOR *) Destination;
    do {
      Source = (void *)((uint8_t *) Source + sizeof (MPIO_PORT_DESCRIPTOR));
      if (((MPIO_PORT_DESCRIPTOR *)Source)->EngineData.EngineType != MpioUnusedEngine) {
        memcpy (Destination, Source, sizeof (MPIO_PORT_DESCRIPTOR));
        Destination = (void *)((uint8_t *) Destination + sizeof (MPIO_PORT_DESCRIPTOR));
      }
    } while ((((MPIO_PORT_DESCRIPTOR *)Source)->Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );
  }

  return EFI_SUCCESS;
}

/**
 * MpioDataBackToHostFW
 *
 * @brief Send Updated MPIO IP block's data to host FW
 * @return EFI_SUCCESS or EFI_NOT_FOUND
 *       EFI_SUCCESS   : Received valid address within the Host allocated memory block
 *                       and succesfully update the PCD.
 *       EFI_NOT_FOUND : Indicates the requested block was not found
 */
EFI_STATUS
MpioDataBackToHostFW (
  void
  )
{
  EFI_STATUS status;
  MPIOCLASS_INPUT_BLK *MpioDataHostFw;
  PEI_AMD_NBIO_PCIE_COMPLEX_PPI *NbioPcieComplexPpi;
  UINT32                        Idx0, Idx1;
  MPIO_COMPLEX_DESCRIPTOR       *CpxDescSrc;
  MPIO_PORT_DESCRIPTOR          *PortDescSrc;
  MPIO_DDI_DESCRIPTOR           *DdiDescSrc;
  DXIO_COMPLEX_DESCRIPTOR       *CpxDescDest;
  DXIO_PORT_DESCRIPTOR          *PortDescDest;
  DDI_DESCRIPTOR                *DdiDescDest;
  DXIO_COMPLEX_DESCRIPTOR       *PcieTopologyData;
  CONST EFI_PEI_SERVICES        **PeiServices;

  MpioDataHostFw = (MPIOCLASS_INPUT_BLK *)SilFindStructure (SilId_MpioClass,  0);
  DEBUG ((DEBUG_ERROR, "MpioDataBackToHostFW SIL MPIO memory block at: 0x%x \n", MpioDataHostFw));
  if (MpioDataHostFw == NULL) {
    return EFI_NOT_FOUND; // Could not find the IP input block
  }

  status = PcdSet8S(PcdAmdMCTPMasterSeg, MpioDataHostFw->AmdMCTPMasterSeg);
  DEBUG ((DEBUG_ERROR, "PcdAmdMCTPMasterSeg update: %d\n", status));
  status = PcdSet16S(PcdAmdMCTPMasterID, MpioDataHostFw->AmdMCTPMasterID);
  DEBUG ((DEBUG_ERROR, "PcdAmdMCTPMasterID update: %d\n", status));

  PeiServices = GetPeiServicesTablePointer ();
  status = (*PeiServices)->LocatePpi ((CONST EFI_PEI_SERVICES **)PeiServices,
              &gAmdNbioPcieComplexPpiGuid, 0, NULL, (VOID **)&NbioPcieComplexPpi);
  if (EFI_ERROR (status)) {
    DEBUG ((DEBUG_ERROR, "gAmdNbioPcieComplexPpiGuid NOT installed!!\n"));
    assert (false);
  }
  NbioPcieComplexPpi->PcieGetComplex (NbioPcieComplexPpi, &PcieTopologyData);

  CpxDescSrc  = MpioDataHostFw->PcieTopologyData.PlatformData;
  CpxDescDest = PcieTopologyData;

  Idx0 = 0;
  do {
    CpxDescDest[Idx0].Flags           = CpxDescSrc[Idx0].Flags;
    CpxDescDest[Idx0].SocketId        = CpxDescSrc[Idx0].SocketId;
    CpxDescDest[Idx0].BmcLinkLocation = CpxDescSrc[Idx0].BmcLinkLocation;
    CpxDescDest[Idx0].BmcLinkLaneNum  = CpxDescSrc[Idx0].BmcLinkLaneNum;

    Idx1 = 0;
    PortDescSrc  = CpxDescSrc[Idx0].PciePortList;
    PortDescDest = CpxDescDest[Idx0].PciePortList;
    do {
      memcpy((void *)&PortDescDest[Idx1], (void *)&PortDescSrc[Idx1], sizeof(MPIO_PORT_DESCRIPTOR));
      Idx1++;
    } while ((PortDescSrc[Idx1-1].Flags & DESCRIPTOR_TERMINATE_LIST) == 0);

    Idx1 = 0;
    DdiDescSrc  = CpxDescSrc[Idx0].DdiLinkList;
    DdiDescDest = CpxDescDest[Idx0].DdiLinkList;
    do {
      memcpy((void *)&DdiDescDest[Idx1], (void *)&DdiDescSrc[Idx1], sizeof(MPIO_DDI_DESCRIPTOR));
      Idx1++;
    } while ((DdiDescSrc[Idx1-1].Flags & DESCRIPTOR_TERMINATE_LIST) == 0);

    Idx0++;
  } while ((CpxDescSrc[Idx0-1].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0);

  return EFI_SUCCESS;
}
