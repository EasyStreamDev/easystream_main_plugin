#!/usr/bin/env bash
rootProject="$(git rev-parse --show-toplevel)"
compileResource="${rootProject}/compileResource"
obsFolder="${rootProject}/compileResource/obs-studio"
buildDir="${obsFolder}/build_x86_64"
easyStreamEnv="${rootProject}/easyStreamEnv"