#!/usr/bin/env bash

pip install virtualenv
virtualenv -p python3 easyStreamEnv
cd $easyStreamEnv
source ./bin/activate
cd ..
pip install conan==1.59.0