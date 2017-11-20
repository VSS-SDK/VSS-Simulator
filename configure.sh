#!/bin/bash
#
# This file is part of the VSS-SDK project.
#
# This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
# v. 3.0. If a copy of the GPL was not distributed with this
# file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
#

DISTRO=`lsb_release -si`
RELEASE=`lsb_release -sr`
ARCHITECTURE=`uname -m`

INSTALLED=0

CMAKE_UBUNTU () {
  sudo rm -R build
  mkdir build
  cd build
  cmake ..
  make package
  sudo dpkg -i vss-simulator-0.1.1-Linux.deb
  cd ..
}

CMAKE_DEBIAN () {
  rm -R build
  mkdir build
  cd build
  cmake ..
  make package
  dpkg -i vss-simulator-0.1.1-Linux.deb
  cd ..
}

INSTALL_UBUNTU_14_04 () {
  sudo apt-get update && apt-get upgrade
  sudo apt-get install pkg-config
  sudo apt-get install g++ cmake libzmq3 libzmq3-dev protobuf-compiler libprotobuf-dev libboost-all-dev
  INSTALLED=1
}

INSTALL_UBUNTU_16_04 () {
  sudo apt-get update && apt-get upgrade
  sudo apt-get install pkg-config
  sudo apt-get install g++ cmake libzmqpp3 libzmqpp-dev protobuf-compiler libprotobuf-dev libboost-all-dev
  INSTALLED=1
}

INSTALL_UBUNTU_17_10 () {
  sudo apt-get update && apt-get upgrade
  sudo apt-get install pkg-config
  sudo apt-get install g++ cmake libzmq5 libzmqpp4 libzmqpp-dev protobuf-compiler libprotobuf-dev libboost-all-dev
  INSTALLED=1
}

INSTALL_DEBIAN_8_2 () {
  sudo apt-get update && apt-get upgrade
  sudo apt-get install pkgconf
  sudo apt-get install g++ cmake libzmq3 libzmq3-dev protobuf-compiler libprotobuf-dev libboost-all-dev
  INSTALLED=1
}

INSTALL_DEBIAN_8_5 () {
  sudo apt-get update && apt-get upgrade
  sudo apt-get install pkgconf
  sudo apt-get install g++ cmake libzmq3 libzmq3-dev protobuf-compiler libprotobuf-dev libboost-all-dev
  INSTALLED=1
}

INSTALL_DEBIAN_9_2 () {
  sudo apt-get update && apt-get upgrade
  sudo apt-get install pkgconf
  sudo apt-get install g++ cmake libzmq5 libzmq3-dev protobuf-compiler libprotobuf-dev libboost-all-dev
  INSTALLED=1
}

INSTALL () {
  # Ubuntu
  if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "17.10" ]]; then
    INSTALL_UBUNTU_17_10;
    if [ $INSTALLED == 1 ]; then
      CMAKE_UBUNTU;
    fi
  fi
  if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "16.04" ]]; then
    INSTALL_UBUNTU_16_04;
    if [ $INSTALLED == 1 ]; then
      CMAKE_UBUNTU;
    fi
  fi
  if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "14.04" ]]; then
    INSTALL_UBUNTU_14_04;
    if [ $INSTALLED == 1 ]; then
      CMAKE_UBUNTU;
    fi
  fi

  # Debian
  if [[ "$DISTRO" == "Debian" ]] && [[ "$RELEASE" == "9.2" ]]; then
    INSTALL_DEBIAN_9_2;
    if [ $INSTALLED == 1 ]; then
      CMAKE_DEBIAN;
    fi
  fi
  if [[ "$DISTRO" == "Debian" ]] && [[ "$RELEASE" == "8.5" ]]; then
    INSTALL_DEBIAN_8_5;
    if [ $INSTALLED == 1 ]; then
      CMAKE_DEBIAN;
    fi
  fi
  if [[ "$DISTRO" == "Debian" ]] && [[ "$RELEASE" == "8.2" ]]; then
    INSTALL_DEBIAN_8_2;
    if [ $INSTALLED == 1 ]; then
      CMAKE_DEBIAN;
    fi
  fi
}

INSTALL;
