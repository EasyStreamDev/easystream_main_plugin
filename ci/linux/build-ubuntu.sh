#!/usr/bin/env bash
cd $easyStreamEnv
source ./bin/activate
cd $rootProject

if [ $# -eq 0 ]; then
    echo "No arguments given"
    exit -1
fi

# set -ex
if [[ $2 == true ]]; then
    rm -rf build && mkdir build && cd build
    conan profile detect --force
    if [ "$1" = "Debug" ]; then
        conan install "$rootProject/utils/ubuntu/" --output-folder=. --build=missing -pr "$rootProject/utils/ubuntu/ubuntuDebug"
    else
        conan install "$rootProject/utils/ubuntu/" --output-folder=. --build=missing -pr "$rootProject/utils/ubuntu/ubuntuRelease"
    fi
    cmake -DCMAKE_TOOLCHAIN_FILE="$rootProject/build/conan_toolchain.cmake"  -DCMAKE_BUILD_TYPE="$1" ..
else
    cd build
fi
cmake --build . --config "$1"