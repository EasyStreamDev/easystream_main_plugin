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
    [string]$Setup,

    [Parameter()]
    [switch]$IsOnCI

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

    if (Test-Path -Path "${rootDir}/build") {
        Get-ChildItem "build" -Recurse | Remove-Item -Force -Recurse
    } else {
        mkdir "${rootDir}/build"
    }
    Set-Location "${rootDir}/build"
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
    mkdir $obsFolder

    if ($Dependencies.IsPresent) {
        # installBoost $BoostFolder
        installConan
    }
    if ($CloneObs.IsPresent) {
        if ($IsOnCI.IsPresent) {
            getObsCi -obsFolder $obsFolder -buildMode $BuildTypeObs
        } else {
            getObs -obsFolder $obsFolder -buildMode $BuildTypeObs
        }
    }
    buildEasyStream

    if ($Setup -eq 'Setup') {
        generateSetup -buildPath $rootDir
    } elseif ($Setup -eq 'Source') {
        generateSetup -buildPath $buildFolder -source
    }
}

main