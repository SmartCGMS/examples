# <img src="https://diabetes.zcu.cz/img/icon.png" width="24" height="24" /> SmartCGMS - examples
This repository contains example implementations and configurations for the SmartCGMS software framework and architecture

SmartCGMS software architecture and framework.
Project homepage: [diabetes.zcu.cz](https://diabetes.zcu.cz/smartcgms)
## Repository structure
- `entities` - example SmartCGMS entities, such as filters, models and other
- `experimental-setups` - examples of configurations and experimental setups to run the SmartCGMS in various modes
- `frontends` - examples of SmartCGMS front-end implementations
- `misc` - miscellaneous implementations and configurations, e.g., foreign language bindings and more
- `native-scripts` - examples of native scripts for the use with the [native scripting filter](https://diabetes.zcu.cz/docs/entities/miscfilters)

## Build

If you either want to build the whole examples directory, or a single example on its own, you need to clone the "common" repository so you can find it later. Then, you can choose to build the whole examples directory using the top-level CMakeLists file, or a single example using its own CMakeLists file.

During the configuration, add the "common" directory path to the `SMARTCGMS_COMMON_DIR` variable, e.g.:

```
mkdir build
cd build
cmake ../ -DSMARTCGMS_COMMON_DIR=/home/myuser/scgms/common/
cmake --build .
```

All examples are configured to directly include files from "common" in their build, so there is no need to link the static version of the "common" library.

## License

The SmartCGMS software and its components are distributed under the Apache license, version 2. When publishing any derivative work or results obtained using this software, you agree to cite the following paper:

_Tomas Koutny and Martin Ubl_, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science, Volume 177, pp. 354-362, 2020

See attached LICENSE file for full licencing information.

|![University of West Bohemia](https://www.zcu.cz/en/assets/logo.svg)|![Department of Computer Science and Engineering](https://www.kiv.zcu.cz/site/documents/verejne/katedra/dokumenty/dcse-logo-barevne.png)|
|--|--|