#!/usr/bin/sh

CURR_DIR=`pwd`
APP_VER="v$(cat ./.last_version)"
APP_DIR="./NvCtrl-Linux.AppDir"
BIN_DIR="$1"

cd $BIN_DIR && cmake .. && make clean && make -j$(nproc)
cd $CURR_DIR
cp $BIN_DIR/NvCtrl-Linux $APP_DIR/usr/bin/.

appimagetool $APP_DIR
mv NvCtrl-Linux-x86_64.AppImage NvCtrl-Linux-$APP_VER-x86_64.AppImage

