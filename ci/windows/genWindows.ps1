param(
    [Parameter()]
    [switch]$Help,

    [Parameter()]
    [switch]$Dependencies,

    [Parameter()]
    [switch]$CloneObs,

    [Parameter()]
    [validateSet('x86', 'x64')]
    [string]$BuildArch
)


$rootDir = Resolve-Path -Path "$PSScriptRoot\..\.."
$BoostFolder = "${rootDir}/compileResource/boostFolder/"
$obsFolder = "${rootDir}/compileResource/"

.${PSScriptRoot}/installBoost.ps1
.${PSScriptRoot}/installConan.ps1
.${PSScriptRoot}/getObs.ps1

function printUsage {
    $Usage = @(
        "Usage : $($MyInvocation.MyCommand.Name)",
        "-Help          :   Print Usage",
        "-Dependencies  :   Download dependencies of easystream",
        "-Clone         :   Clone and build obs-studio"
    )
    $Usage | Write-Host
}

function buildEasyStream {
    Set-Location $rootDir
    if (Test-Path -Path "build") {
        Get-ChildItem "build" -Recurse | Remove-Item -Force -Recurse
    }
    Set-Location "build"
    conan.exe install ../ --profile ../utils/easystream
    cmake -G "Visual Studio 16 2019" ..
    cmake --build ./
}

function main {
    if ($Help.IsPresent) {
        printUsage
        exit 0
    }
    if ($Dependencies.IsPresent) {
        installBoost $BoostFolder
        installConan
    }
    if ($CloneObs.IsPresent) {
        if ($PSBoundParameters.ContainsKey('BuildArch')) {
            getObs -obsFolder $obsFolder -Arch $BuildArch
        } else {
            $arch = ('x86', 'x64')[[System.Environment]::Is64BitOperatingSystem]
            getObs -obsFolder $obsFolder -Arch $arch
        }
    }
    buildEasyStream
}

main