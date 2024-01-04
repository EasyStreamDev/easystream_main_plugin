#!/usr/bin/env bash

mkdir -p "$compileResource"
cd "$compileResource"

install_aur_package() {
  package_name=$1

  # Clone the AUR repository
  git clone "https://aur.archlinux.org/$package_name.git"

  # Enter the repository directory
  cd "$package_name"

  # Update the repository
  git pull

  # Build and install the package
  makepkg -si --noconfirm

  # Return to the previous directory
  cd ..
}

if [ ! -d "obs-studio-git" ]; then
    if [ ! -d "cef-minimal-obs-bin" ]; then
        install_aur_package "cef-minimal-obs-bin"
    fi

    if [ ! -d "libajantv2" ]; then
        install_aur_package "libajantv2"
    fi

    if [ ! -d "librist" ]; then
        install_aur_package "librist"
    fi

    if [ ! -d "libdatachannel" ]; then
        if [ ! -d "libjuice" ]; then
            install_aur_package "libjuice"
        fi

        install_aur_package "libdatachannel"
    fi

    if [ ! -d "qrcodegen-cmake" ]; then
        install_aur_package "qrcodegen-cmake"
    fi

    install_aur_package "obs-studio-git"
fi


cd "./obs-studio-git/src/obs-studio-git/build"
sudo cmake -P cmake_install.cmake
sudo cp /usr/lib/cmake/libobs/libobsConfig.cmake /usr/lib/cmake/libobs/LibObsConfig.cmake