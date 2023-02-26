#!/usr/bin/env bash
cd $rootProject
# set -ex
rm -rf build && mkdir build && cd build
conan install "$rootProject/utils/ubuntu/" --build=missing --profile "$rootProject/utils/ubuntu/ubuntuDebug"
cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_UBUNTU=yes ..
make -j4