#!/usr/bin/env bash
cd $easyStreamEnv
echo "Entering env folder"
echo $PWD
source ./bin/activate
echo "Environment activated"
cd $rootProject

if [ $# -eq 0 ]; then
    echo "No arguments given"
    exit -1
fi

# set -ex
if [[ $2 == true ]]; then
    rm -rf build && mkdir build && cd build
    if [ "$1" = "Debug" ]; then
        conan install "$rootProject/utils/ubuntu/" --build=missing --profile "$rootProject/utils/ubuntu/ubuntuDebug"
    else
        conan install "$rootProject/utils/ubuntu/" --build=missing --profile "$rootProject/utils/ubuntu/ubuntuRelease"
    fi
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_ARCH=yes -DCMAKE_BUILD_TYPE="$1" ..
else
    cd build
fi
cmake --build . --config "$1"
# cmake --build --preset linux-x86_64