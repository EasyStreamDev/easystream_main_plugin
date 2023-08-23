function getObs {
    param(
        [Parameter(Mandatory)]
        [string]$obsFolder,

        [Parameter(Mandatory)]
        [string]$buildMode
    )

    if (Test-Path -Path "${obsFolder}/obs-studio") {
        Set-Location "${obsFolder}/obs-studio/"
        git pull
    } else {
        Set-Location $obsFolder
        git clone --recursive https://github.com/obsproject/obs-studio.git
        Set-Location "obs-studio"
    }
    cmake --preset windows-x64 -DCMAKE_BUILD_TYPE=$buildMode
    cmake --build --preset windows-x64
}