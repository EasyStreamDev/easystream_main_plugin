#!/usr/bin/env bash
# scriptRoot = $(dirname "$0")
# projectRoot = "${scriptRoot}/../.."
# compileFolder = "${projectRoot}/compileResource"
# buildType="debug"
# buildDir = "../../obsBuildDir"



# while getopts bt:p flag
# do
#     case $flag in
#         bt) buildType=${OPTARG};;
#         p) buildDir=${OPTARG};;
#     esac
# done

mkdir -p "$compileResource"

cd "$compileResource"

if [ ! -d $buildDir ]; then
    mkdir obsBuildDir
fi

if [ ! -d $obsFolder ]; then
    git clone --recursive https://github.com/obsproject/obs-studio.git
fi

cd $obsFolder
git pull
# cd .github/scripts/
# chmod +x build-linux
# ./build-linux -c "$1"  -t "linux-x86_64"
# cd $buildDir
if [$1 -eq "Release"]; then
    cmake --preset linux-release-x86_64 -DCMAKE_BUILD_TYPE=$1 -DENABLE_BROWSER=OFF -DENABLE_AJA=OFF -DENABLE_WEBRTC=OFF -DENABLE_NEW_MPEGTS_OUTPUT=OFF
    cmake --build --preset linux-release-x86_64
else
    cmake --preset linux-x86_64 -DCMAKE_BUILD_TYPE=$1 -DENABLE_BROWSER=OFF -DENABLE_AJA=OFF -DENABLE_WEBRTC=OFF -DENABLE_NEW_MPEGTS_OUTPUT=OFF
    cmake --build --preset linux-x86_64
fi

cd "build_x86_64"
sudo cmake -P cmake_install.cmake
sudo cp /usr/local/lib/cmake/libobs/libobsConfig.cmake /usr/local/lib/cmake/libobs/LibObsConfig.cmake
