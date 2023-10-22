#!/usr/bin/env bash
package=false
skip=false
buildType="Debug"
rebuild=true
ci_env=false

while getopts cpsr:b flag;
do
    case "$flag" in
        c) ci_env=true;;
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
    source "${rootProject}/ci/linux/setup-py.sh"
    if [[ $ci_env == false ]]; then
        source "${rootProject}/ci/linux/build-obs.sh" $buildType
    else
        source "${rootProject}/ci/linux/compile-ci.sh" $buildType
    fi
fi

export PATH="$HOME/.local/bin:$PATH"
source "${rootProject}/ci/linux/build-ubuntu.sh" $buildType $rebuild

if [[ $package == true ]]; then
    source "${rootProject}/ci/linux/package.sh"
fi