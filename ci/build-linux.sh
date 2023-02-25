package = false

while getopts p flag
do
    case $flag in
        p) package=true;;
    esac
done


source "${rootProject}/ci/linux/variables.sh"

source "${rootProject}/ci/linux/install-dependencies-ubuntu.sh"

source "${rootProject}/ci/linux/build-obs.sh"

source "${rootProject}/ci/linux/build-ubuntu.sh"

if [ "$package" = true]; then
    source "${rootProject}/ci/linux/package-ubuntu.sh"
fi