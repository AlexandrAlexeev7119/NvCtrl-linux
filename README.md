# GWEpp
GreenWithEnvy re-implementation using only C++ and Qt

no other extra dependencies such as Python and XNvCtrlLib
![UI 1](img/ui1.png)

## Installation
* Download AppImage file from [latest release](https://codeberg.org/AlexCr4ckPentest/NvCtrl-linux/releases/latest)


### Building from sources
```
$ git clone --recurse-submodules https://codeberg.org/AlexCr4ckPentest/NvCtrl-linux
$ cd GWEpp
$ mkdir build && cd build
$ cmake ..
$ make -j$(nproc)
```
