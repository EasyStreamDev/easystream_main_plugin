#!/usr/bin/env bash

echo "---------------- Creating environment"
python -m venv easyStreamEnv
echo "---------------- Done"
cd $easyStreamEnv
echo $PWD
source ./bin/activate
cd ..
pip install conan==1.59.0
