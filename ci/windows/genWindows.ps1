param(
    [Parameter()]
    [switch]$Help,

    [Parameter()]
    [switch]$Dependencies,

    [Parameter()]
    [switch]$CloneObs,

    [Parameter(Mandatory)]
    [ValidateSet("Release", "RelWithDebInfo", "MinSizeRel", "Debug")]
    [string]$BuildTypeObs,

    [Parameter(Mandatory)]
    [ValidateSet("Release", "Debug")]
    [string]$BuildType,

    [Parameter()]
    [validateSet('x86', 'x64')]
    [string]$BuildArch,

    [Parameter(Mandatory)]
    [ValidateSet('Setup', 'Source', 'None')]
    [string]$Setup
)

$rootDir = Resolve-Path -Path "$PSScriptRoot\..\.."
$buildFolder = "${rootDir}/build"
$obsFolder = "${rootDir}/compileResource/"

.${PSScriptRoot}/installBoost.ps1
.${PSScriptRoot}/installConan.ps1
.${PSScriptRoot}/getObs.ps1
.${PSScriptRoot}/package.ps1

function printUsage {
    $Usage = @(
        "Usage : $($MyInvocation.MyCommand.Name)",
        "-Help          :   Print Usage",
        "-Dependencies  :   Download dependencies of easystream",
        "-Clone         :   Clone and build obs-studio"
        "-Release       :   Make a release instead"
    )
    $Usage | Write-Host
}

function buildEasyStream {
    Set-Location $rootDir
    if (Test-Path -Path "build") {
        Get-ChildItem "build" -Recurse | Remove-Item -Force -Recurse
    } else {
        mkdir "build"
    }
    Set-Location "build"
    # if ($BuildType == )
    if ($BuildType -eq "Release") {
        conan.exe install ../utils/windows/ --profile ../utils/windows/windowsRelease --build=missing
    } else {
        conan.exe install ../utils/windows/ --profile ../utils/windows/windowsDebug --build=missing
    }
    cmake -G "Visual Studio 17 2022" .. -DCMAKE_BUILD_TYPE="$BuildTypeObs"
    cmake --build ./ --config $BuildType
}

function main {
    if ($Help.IsPresent) {
        printUsage
        exit 0
    }
    if ($Dependencies.IsPresent) {
        # installBoost $BoostFolder
        installConan
    }
    if ($CloneObs.IsPresent) {
        if ($PSBoundParameters.ContainsKey('BuildArch')) {
            getObs -obsFolder $obsFolder -Arch $BuildTypeObs
        } else {
            $arch = ('x86', 'x64')[[System.Environment]::Is64BitOperatingSystem]
            getObs -obsFolder $obsFolder -Arch $arch -buildMode $BuildTypeObs
        }
    }
    buildEasyStream

    if ($Setup -eq 'Setup') {
        generateSetup -buildPath $buildFolder
    } elseif ($Setup -eq 'Source') {
        generateSetup -buildPath $buildFolder -source
    }
}

main