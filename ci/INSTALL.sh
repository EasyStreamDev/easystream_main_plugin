#!/bin/bash

ORANGE='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'
url="https://github.com/EasyStreamDev/easystream_main_plugin/releases/latest/download/libobs-easystream-main-plugin.so"


echo "=========================================="
echo "=                                        ="
echo -e "=          ${ORANGE}Easy${BLUE}Stream Installer${NC}          ="
echo "=                                        ="
echo "=========================================="


while true 
do
    read -p "Do you want to install OBS? " -n 1 -r
    # read -p "y or n" obs
    if [[ $REPLY =~ ^[Yy]$ ]] 
    then
        sudo add-apt-repository ppa:obsproject/obs-studio
        sudo apt install obs-studo
        break
    elif [[ $REPLY =~ ^[Nn]$ ]]
    then
        break
    fi
done

echo "Now Installing EasyStream"
sudo apt install curl
curl "$url" --output libobs-easystream-main-plugin.so
sudo cp ./libobs-easystream-main-plugin.so /usr/local/lib/obs-plugins/

echo "Do you want to install The transcription Module"
while true 
do
    read -p "c for Echostra+cuda, y for Echostra and n for no" -n 1 -r
    if [[ $REPLY =~ ^[Cc]$ ]] 
    then
        cd ~/
        sudo apt install nvidia-cuda-toolkit
        sudo apt install git
        git clone git@github.com:EasyStreamDev/Echostra.git
        cd Echostra
        git checkout cuda
        pip install virtualenv
        virtualenv EasyStreamEnv
        source ./easystreamEnv/scripts/Activate
        pip install -r ./requirements_linux.txt
        pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
        break
    elif [[ $REPLY =~ ^[Yy]$ ]]
    then
        cd ~/
        sudo apt install git
        git clone git@github.com:EasyStreamDev/Echostra.git
        cd Echostra
        pip install virtualenv
        virtualenv EasyStreamEnv
        source ./easystreamEnv/scripts/Activate
        pip install -r ./requirements_linux.txt
        pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu
        break
    elif [[ $REPLY =~ ^[Nn]$ ]]
    then
        break
    fi
done