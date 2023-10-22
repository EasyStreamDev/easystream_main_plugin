function installConan {
    param(
        [Parameter(Mandatory)]
        [string]$EnvFolder,

        [Parameter(Mandatory)]
        [bool]$Release
    )


    $pyEx = &{python -V} 2>&1
    $version = if($pyEx -is [System.Management.Automation.ErrorRecord]) {
        winget.exe install -e --id Python.Python.3.11
    } else {
        $pyEx
    }
    Set-Location $EnvFolder
    py -m ensurepip --upgrade
    pip install --user virtualenv
    python -m venv easystreamEnv
    ./easystreamEnv/scripts/Activate.ps1
    pip install conan==1.59.0
    pip install -r ./requirements_win.txt
    # Set-Location "../"
    # $currDir = Get-Location
    # Write-Host $PSScriptRoot
}