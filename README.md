# README

## Beta Release of UEFI Host Firmware for Genoa openSIL POC (opensil-uefi-interface)

Thank you for your interest in the testing of this beta firmware. This firmware is provided to you as-is and is under the terms of the [beta license agreement](https://github.com/openSIL/opensil-uefi-interface/blob/main/2023.10.9%20Beta%20Firmware%20License%20(source%20code)%20v.2.pdf) that is available in this repository.


### Disclaimer

Please note that the content available in [this](https://github.com/openSIL/opensil-uefi-interface) repository is a beta version of the opensil-uefi-interface (a component of a sample UEFI Host Firmware for Genoa openSIL POC), which means it is still in a development and testing phase. While we have done our best to ensure the stability of this set of firmware, it may still contain bugs, incomplete features, or other issues.

### License

By using this beta firmware, you agree to the terms and conditions of the [Beta Firmare license agreement](https://github.com/openSIL/opensil-uefi-interface/blob/main/2023.10.9%20Beta%20Firmware%20License%20(source%20code)%20v.2.pdf).


### Intended usage

The beta release of the opensil-uefi-interface is intended to integrate with the AMD openSIL Genoa Proof-Of-Concept (POC), which is available at https://github.com/openSIL/openSIL.


## About the AMD openSIL opensil-uefi-interface

## Overview

This repository provides the necessary files and references for integrating AMD openSIL into a UEFI project.

## Components of opensil-uefi-interface
- The AMD openSIL Genoa POC, included as a submodule [submodule "openSIL"]
- The SilToUefi folder, which houses UEFI modules added to PEI execution. These modules contain calls to OpenSIL for various silicon IP initialization tasks.
- AmdOpenSilPkg.dec, a UEFI package declaration file.  The file name suggests the directory for the OpenSIL package, *AmdOpenSilPkg*. Note that the correlation between the DEC file name and package directory is optional; the contents of this repository can be placed in any directory within the UEFI project.
- INF files, which list the source files for xSIM, xUSL, and xPRF libraries.  These INF files should be included in the target DSC file of the UEFI project.

## Integrating OpenSIL UEFI Package into Your Project

- **Step 1:** In your UEFI project, create a folder for the UEFI OpenSIL package, e.g., $(PROJECT_DIR)/AmdOpenSilPkg.

- **Step 2:** Clone the repository from the package folder:

  `git clone git@github.com:AMD-OpenSIL/opensil-uefi-interface.git --recursive`

- **Step 3:** Add references to the new INF files in the project's DSC file.

Alternatively, you can present the opensil-uefi-interface repository as a submodule in a parent UEFI project repository.  Cloning this parent project with the `--recursive` option will automatically perform Steps 1 through 3.

## Memory Allocation for AMD openSIL

The opensil-uefi-interface allocates a single block of contiguous memory through the creation of a GUIDed HOB (hand-off-block).  This HOB contains the contents of the AMD openSIL IP block data, which is populated during the SIL initialization prior to AMD openSIL time point 1 execution. The host is responsible for relocating this data, if necessary, and providing the base address to each AMD openSIL time point.

The opensil-uefi-interface defines the following GUID, which represents the name of this openSIL HOB:

gPeiOpenSilDataHobGuid = { 0xbc6ef377, 0x4190, 0x47d1, { 0x84, 0xe3, 0xe3, 0xc7, 0xa0, 0x8f, 0x35, 0xb8 }}
