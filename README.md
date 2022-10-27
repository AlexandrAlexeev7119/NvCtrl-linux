# GWEpp
GreenWithEnvy re-implementation using only C++ and Qt

no other extra dependencies such as Python

## UI preview
![UI 1](img/ui1.png)
![UI 2](img/ui2.png)
![UI 3](img/ui3.png)

## Installation
### Arch GNU/Linux (creating native package)
* Download PKGBUILD file from [latest release](https://github.com/AlexandrAlexeev7119/GWEpp/releases/latest/download/PKGBUILD)
* Place PKGBUILD file to dedicated directory (for example: GWEpp)
* run `makepkg -si`

### Other distribution
* Download AppImage file from [latest release](https://github.com/AlexandrAlexeev7119/GWEpp/releases/latest/download/GWEpp-x86_64.AppImage )


### Building from sources
```
$ git clone --recurse-submodules https://notabug.org/AlexCr4ckPentest/GWEpp
$ cd GWEpp
$ mkdir build && cd build
$ cmake ..
$ make -j$(nproc)
```
