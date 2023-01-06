function getObs {
    param(
        [Parameter(Mandatory)]
        [string]$obsFolder,

        [Parameter(Mandatory)]
        [validateSet('x86', 'x64')]
        [string]$Arch
    )

    if (Test-Path -Path "obs-studio") {
        Set-Location "${obsFolder}/obs-studio"
        git pull
    } else {
        Set-Location $obsFolder
        git clone --recursive https://github.com/obsproject/obs-studio.git
        Set-Location "obs-studio"
    }
    ./CI/build-windows.ps1 -BuildArch $Arch
}