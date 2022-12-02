#!/usr/bin/bash

### NvCtrl-linux install script
# Install binaries in /usr/bin/ and copy .desktop entry in /usr/share/applications/

BINDIR="/usr/bin/"

function check_binaries()
{
  if [[ ! -d ./build ]] || [[ ! -f ./build/NvCtrl-Linux ]]
  then
    echo "Please, run build.sh first"
    exit 1
  fi
}

function install_binaries()
{
  cp ./build/NvCtrl-Linux $BINDIR/.
  cp ./NvCtrl-Linux.desktop /usr/share/applications/.
  echo "NvCtrl-Linux binaries are installed in $BINDIR"
  echo "To remove application, run: ./install.sh --uninstall"
}

if [[ "$1" == "--uninstall" ]] || [[ "$1" == "--remove" ]]
then
  rm /usr/bin/NvCtrl-Linux
  rm /usr/share/applications/NvCtrl-Linux.desktop
else
  check_binaries
  install_binaries
fi

