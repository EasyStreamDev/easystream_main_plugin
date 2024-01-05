function generateSetup {
    param (
        [Parameter(Mandatory)]
        [string]$buildPath,

        [Parameter()]
        [switch]$source
    )

    Set-Location "${buildPath}"
    makensis.exe .\test.nsi
}