#!/usr/bin/env bash
skip=false
buildType="Debug"
rebuild=true

while getopts psr:b flag;
do
    case "$flag" in
        b) rebuild=false;;
        s) skip=true;;
        r) buildType="$OPTARG";;
    esac
done

variableScript="$(dirname "$0")/arch/variables.sh"

source $variableScript

# set -ex

if [[ $skip == false ]]; then
    source "${rootProject}/ci/arch/install-dependencies-arch.sh"
    source "${rootProject}/ci/arch/setup-py.sh"
    source "${rootProject}/ci/arch/build-obs.sh" $buildType
fi

export PATH="$HOME/.local/bin:$PATH"
source "${rootProject}/ci/arch/build-manjaro.sh" $buildType $rebuild
