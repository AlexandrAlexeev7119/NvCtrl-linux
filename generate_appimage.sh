#!/usr/bin/sh

CURR_DIR=`pwd`
APP_VER="v$(cat ./.last_version)"
APP_DIR="./GWEpp.AppDir"
BIN_DIR="./build-GWEpp-GCC-Release"

cd $BIN_DIR && cmake .. && make clean && make -j$(nproc)
cd $CURR_DIR
cp $BIN_DIR/gwepp $APP_DIR/usr/bin/.

appimagetool $APP_DIR
mv GWEpp-x86_64.AppImage GWEpp-$APP_VER-x86_64.AppImage

