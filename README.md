# README

## Beta Release of UEFI Host Firmware for Turin openSIL POC (opensil-uefi-interface)

Thank you for your interest in the testing of this beta firmware. This firmware is provided to you as-is and is under the terms of the [beta license agreement](https://github.com/openSIL/opensil-uefi-interface/blob/turin_poc/2023.11.20%20Beta%20Firmware%20License%20-%20Turin%20(source%20code).pdf) that is available in this repository.

### Disclaimer

Please note that the content available in [this](https://github.com/openSIL/opensil-uefi-interface) repository is a beta version of the opensil-uefi-interface (a component of a sample UEFI Host Firmware for Turin openSIL POC), which means it is still in a development and testing phase. While we have done our best to ensure the stability of this set of firmware, it may still contain bugs, incomplete features, or other issues.

### License

By using this beta firmware, you agree to the terms and conditions of the [Beta Firmware license agreement](https://github.com/openSIL/opensil-uefi-interface/blob/turin_poc/2023.11.20%20Beta%20Firmware%20License%20-%20Turin%20(source%20code).pdf).

### Intended usage

The beta release of the opensil-uefi-interface on the `turin_poc` branch is intended to integrate with the AMD openSIL Turin Proof-Of-Concept (POC), which is available at https://github.com/openSIL/openSIL (branch `turin_poc`).

## About the AMD openSIL opensil-uefi-interface

### Overview

This repository provides the necessary files and references for integrating AMD openSIL (Turin / Breithorn) into a UEFI project.

### Components of opensil-uefi-interface

- The AMD openSIL Turin POC, included as a submodule (`openSIL`) tracking the `turin_poc` branch of https://github.com/openSIL/openSIL
- The `SilToUefi` folder, which provides the UEFI shim library (`SilEfiLib`), C standard headers used by openSIL sources, and supporting assembly helpers
- The `Platform` folder, which contains:
  - Shared PEI / DXE / SMM library implementations under `Platform/Library`
  - Turin platform data-initialization modules under `Platform/Turin` (`SilTurinPei`, `SilTurinDxe`, `SilTurinSmm`)
  - Public headers, PPIs, and protocols under `Platform/Include`
- `AmdOpenSilPkg.dec`, a UEFI package declaration file. The file name suggests the directory for the OpenSIL package, *AmdOpenSilPkg*. Note that the correlation between the DEC file name and package directory is optional; the contents of this repository can be placed in any directory within the UEFI project.
- `AmdOpenSilPkg.dsc` and the Breithorn include fragments (`AmdOpenSilPkgBrh.inc.dsc`, `AmdOpenSilPkgBrh.pei.inc.fdf`, `AmdOpenSilPkgBrh.dxe.inc.fdf`), which show how to wire the package libraries and Turin PEI / DXE / SMM modules into a host firmware build
- INF files (`libF1AM00xSIM.inf`, `libF1AM00xUSL.inf`, `libF1AM00xPRF.inf`, and `libAMDxSIM.inf`), which list the source files for the xSIM, xUSL, and xPRF libraries for the F1AM00 (Turin) SoC. These INF files should be included in the target DSC file of the UEFI project.

## Integrating OpenSIL UEFI Package into Your Project

- **Step 1:** In your UEFI project, create a folder for the UEFI OpenSIL package, e.g., `$(PROJECT_DIR)/AmdOpenSilPkg`.

- **Step 2:** Clone this repository into that package folder on the Turin POC branch, including submodules:

  ```
  git clone --branch turin_poc --recursive https://github.com/openSIL/opensil-uefi-interface.git
  ```

  Or, if cloning via SSH:

  ```
  git clone --branch turin_poc --recursive git@github.com:openSIL/opensil-uefi-interface.git
  ```

- **Step 3:** Add references to the new INF files and Turin components in the project's DSC / FDF files. The Breithorn include fragments in this repository (`AmdOpenSilPkgBrh.inc.dsc`, `AmdOpenSilPkgBrh.pei.inc.fdf`, `AmdOpenSilPkgBrh.dxe.inc.fdf`) illustrate the expected library and module wiring.

Alternatively, you can present the opensil-uefi-interface repository as a submodule in a parent UEFI project repository (pinned to `turin_poc`). Cloning that parent project with the `--recursive` option will automatically perform Steps 1 through 3.

## Memory Allocation for AMD openSIL

The opensil-uefi-interface allocates a single block of contiguous memory through the creation of a GUIDed HOB (hand-off-block). This HOB contains the contents of the AMD openSIL IP block data, which is populated during the SIL initialization prior to AMD openSIL time point 1 execution. The host is responsible for relocating this data, if necessary, and providing the base address to each AMD openSIL time point.

The opensil-uefi-interface defines the following GUID, which represents the name of this openSIL HOB:

```
gPeiOpenSilDataHobGuid = { 0xbc6ef377, 0x4190, 0x47d1, { 0x84, 0xe3, 0xe3, 0xc7, 0xa0, 0x8f, 0x35, 0xb8 }}
```
