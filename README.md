# Two-Particle Correlations in pO Collisions

This repository contains a lightweight, streamlined framework for analyzing two-particle correlations (2PC) in proton-Oxygen (pO) collisions at the LHC using CMS HiForest ROOT files. It is designed to extract $\Delta\eta-\Delta\phi$ 2D correlation histograms, perform event mixing for background subtraction, and ultimately calculate flow coefficients ($v_2$, $v_3$).

## General structure of the repository

The codebase has been highly optimized and stripped of legacy jet and unfolding dependencies. Below is a quick overview of the folder structure:

**Home folder**
Contains the main C++ executable `twopcAnalysis.cxx` and the `Makefile` to compile the framework. It also houses the main configuration cards (e.g., `cardTwoPC.input`) where kinematic cuts and analysis parameters are defined.

**src**
The core C++ classes that drive the analysis:
* `HighForestReader` / `ForestReader`: Streamlined readers to load only the strictly necessary variables (Tracks, Vertex Z) from CMS HiForest files, ensuring fast execution.
* `TwoParticleCorrelationAnalyzer`: Contains the main event loop, track selection, same-event (Signal) pairs calculation, and the buffer logic for mixed-event (Background) pairs.
* `ConfigurationCard`: A flexible parser to read text-based input cards.

**macros / plotting**
Code that does post-processing of the output ROOT files. It includes macros to calculate the Signal/Background ratio ($S/B$), apply the ZYAM (Zero Yield At Minimum) procedure, and fit the 1D $\Delta\phi$ projections with Fourier series to extract $V_{n\Delta}$ coefficients.


---

## Overview of basic workflow

### 1. Test locally that everything works

Before sending any code to GRID processing, you should check that the code compiles and runs successfully on a local test file.

First, compile the framework. The Makefile is configured to link ROOT libraries automatically:
```bash
make clean
make
./twopcAnalysis cardTwoPC_pO.input HiForestMiniAOD_pO.root