function generateSetup {
    param (
        [Parameter(Mandatory)]
        [string]$buildPath,

        [Parameter()]
        [switch]$source
    )

    Set-Location "${buildPath}"
    cpack --config CPackConfig.cmake
    if ($source.IsPresent) {
        cpack --config CPackSourceConfig.cmake
    }
}