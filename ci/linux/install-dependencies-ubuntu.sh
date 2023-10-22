#!/usr/bin/env bash
# scriptRoot = $(dirname "$0")
# projectRoot = "${scriptRoot}/../../"
# cd ../../../




# sudo apt-get -qq update

# sudo add-apt-repository -y ppa:obsproject/obs-studio
# sudo apt-get -qq update

sudo apt-get install -y \
	libc-dev-bin \
	libc6-dev git \
	build-essential \
	checkinstall \
	cmake \
	obs-studio \
	qt6-base-dev \
	libxss-dev \
	libxtst-dev	\
	python-is-python3	\
	python3-pip	\
	zsh

sudo apt install cmake ninja-build pkg-config clang clang-format build-essential curl ccache git zsh
sudo apt install libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libavutil-dev libswresample-dev libswscale-dev libx264-dev libcurl4-openssl-dev libmbedtls-dev libgl1-mesa-dev libjansson-dev libluajit-5.1-dev python3-dev libx11-dev libxcb-randr0-dev libxcb-shm0-dev libxcb-xinerama0-dev libxcb-composite0-dev libxcomposite-dev libxinerama-dev libxcb1-dev libx11-xcb-dev libxcb-xfixes0-dev swig libcmocka-dev libxss-dev libglvnd-dev libgles2-mesa libgles2-mesa-dev libwayland-dev libsrt-openssl-dev libpci-dev libpipewire-0.3-dev libqrcodegencpp-dev
sudo apt install qt6-base-dev qt6-base-private-dev libqt6svg6-dev qt6-wayland qt6-image-formats-plugins
sudo apt install libasound2-dev libfdk-aac-dev libfontconfig-dev libfreetype6-dev libjack-jackd2-dev libpulse-dev libsndio-dev libspeexdsp-dev libudev-dev libv4l-dev libva-dev libvlc-dev libvpl-dev libdrm-dev nlohmann-json3-dev libwebsocketpp-dev libasio-dev libxkbcommon-dev libgl1-mesa-dev
# pip install virtualenv


# sudo pip install conan
	# pip install conan
	# libboost-all-dev \
	# libpipewire-0.3-dev

# # Dirty hack
# sudo wget -O /usr/include/obs/obs-frontend-api.h https://raw.githubusercontent.com/obsproject/obs-studio/25.0.0/UI/obs-frontend-api/obs-frontend-api.h

# if [ ! -d "obs-studio" ]; then
# 	git clone --recursive https://github.com/obsproject/obs-studio.git
# fi

# if [ ! -d "obsBuildDir" ]; then
# 	mkdir obsBuildDir
# fi
# cd obs-studio/CI/
# chmod +x build-linux.sh
# ./build-linux.sh --build-dir ../obsBuildDir
# cd ../../obsBuildDir
# sudo cmake -P cmake_install.cmake
# sudo cp /usr/local/lib/cmake/libobs/libobsConfig.cmake /usr/local/lib/cmake/libobs/LibObsConfig.cmake
# cd ../UI/obs-frontend-api/ &&
# sudo cmake -P cmake_install.cmake &&
# pwd &&
# cd ../../../easystream_main_plugin/
# sudo ldconfig
