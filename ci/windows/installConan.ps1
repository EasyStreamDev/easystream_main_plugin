function installConan {
    $pyEx = &{python -V} 2>&1
    $version = if($pyEx -is [System.Management.Automation.ErrorRecord]) {
        winget.exe install  Python.Python.3.9
    } else {
        $pyEx
    }
    py -m ensurepip --upgrade
    pip install conan==1.59.0
    # Set-Location "../"
    # $currDir = Get-Location
    # Write-Host $PSScriptRoot
}