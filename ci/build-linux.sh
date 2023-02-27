#!/usr/bin/env bash
package=false
skip=false
buildType="Debug"
rebuild=true

while getopts psr:b flag;
do
    case "$flag" in
        b) rebuild=false;;
        p) package=true;;
        s) skip=true;;
        r) buildType="$OPTARG";;
    esac
done

variableScript="$(dirname "$0")/linux/variables.sh"


source $variableScript

# set -ex

if [[ $skip == false ]]; then
    source "${rootProject}/ci/linux/install-dependencies-ubuntu.sh"
    source "${rootProject}/ci/linux/build-obs.sh"
fi

export PATH="$HOME/.local/bin:$PATH"
source "${rootProject}/ci/linux/build-ubuntu.sh" $buildType $rebuild

if [[ $package == true ]]; then
    source "${rootProject}/ci/linux/package-ubuntu.sh"
fi