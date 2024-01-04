#!/usr/bin/env bash

python -m venv easyStreamEnv
cd $easyStreamEnv
echo $PWD
source ./bin/activate
cd ..
pip install conan==1.59.0
