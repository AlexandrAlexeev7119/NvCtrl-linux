#!/usr/bin/bash

### NvCtrl-linux build script

function do_build()
{
  if [[ ! -d build ]]
  then
    mkdir build
  fi
  cd build
  cmake -DCMAKE_BUILD_TYPE=Release ..
  make clean
  make -j$(nproc)
}

do_build

