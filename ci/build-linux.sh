#!/usr/bin/env bash
package=false
skip=false

while getopts "p:s" flag
do
    case "$flag" in
        p) package=true;;
        s) skip=true;;
    esac
done

variableScript="$(dirname "$0")/linux/variables.sh"


source $variableScript

# set -ex
source "${rootProject}/ci/linux/install-dependencies-ubuntu.sh"

if [[ $skip == false ]]; then
    source "${rootProject}/ci/linux/build-obs.sh"
fi

source "${rootProject}/ci/linux/build-ubuntu.sh"

if [[ $package == true ]]; then
    source "${rootProject}/ci/linux/package-ubuntu.sh"
fi