#;*****************************************************************************
#;
#; Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
#;
#;******************************************************************************

################################################################################
#
# Library Class section - list of all Library Classes needed by this Package.
#
################################################################################

[LibraryClasses]
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  SilEfiLib|AmdOpenSilPkg/opensil-uefi-interface/SilToUefi/SilEfiPI72.inf
  FabricResourceInitLib|AgesaRedirects/Library/FabricResourceManagerBrhLib/FabricResourceInit3Lib.inf
  libAMDxSIM|AmdOpenSilPkg/opensil-uefi-interface/libF1AM00xSIM.inf
  libAMDxUSL|AmdOpenSilPkg/opensil-uefi-interface/libF1AM00xUSL.inf
  libAMDxPRF|AmdOpenSilPkg/opensil-uefi-interface/libF1AM00xPRF.inf

[LibraryClasses.common.PEIM]
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  SilPeiInit|AmdOpenSilPkg/opensil-uefi-interface/Platform/Library/Pei/SilPei.inf

[LibraryClasses.common.DXE_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  SilDxeInit|AmdOpenSilPkg/opensil-uefi-interface/Platform/Library/Dxe/SilDxe.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  SilSmmInit|AmdOpenSilPkg/opensil-uefi-interface/Platform/Library/Smm/SilSmm.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Package
#
################################################################################
[PcdsFixedAtBuild]


###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################
[Components]

[Components.IA32]

  AmdOpenSilPkg/opensil-uefi-interface/Platform/Turin/Pei/SilTurinPei.inf {
    <LibraryClasses>
    AmdBaseLib|AgesaModulePkg/Library/AmdBaseLib/AmdBaseLibNoIntrinsic.inf
  }

[Components.X64]
  AmdOpenSilPkg/opensil-uefi-interface/Platform/Turin/Dxe/SilTurinDxe.inf
  AmdOpenSilPkg/opensil-uefi-interface/Platform/Turin/Smm/SilTurinSmm.inf

[BuildOptions]
  GCC:*_*_*_CC_FLAGS     = -D OPENSIL
  INTEL:*_*_*_CC_FLAGS   = /D OPENSIL
  MSFT:*_*_*_CC_FLAGS    = /D OPENSIL
