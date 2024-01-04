#!/bin/bash

packages=(
  git
  cmake
  obs-studio
  qt6-base
  libxss
  libxtst
  python-pip
  zsh
  ninja
  pkg-config
  clang
  base-devel
  curl
  vlc
  ccache
  swig
  qt6-base
  qt6-svg
  qt6-wayland
)

for package in "${packages[@]}"; do
  yay -S --noconfirm "$package"
done
