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
cd CI
chmod +x build-linux.sh
./build-linux.sh --build-dir $buildDir 
cd $buildDir
sudo cmake -P cmake_install.cmake
sudo cp /usr/local/lib/cmake/libobs/libobsConfig.cmake /usr/local/lib/cmake/libobs/LibObsConfig.cmake