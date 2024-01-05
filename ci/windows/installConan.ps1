function installConan {
    # param(
    #     [Parameter(Mandatory)]
    #     [string]$EnvFolder,

    #     [Parameter(Mandatory)]
    #     [bool]$Release
    # )

    # $EnvFolder = "${rootDir}/easystreamEnv"

    $pyEx = &{python -V} 2>&1
    $version = if($pyEx -is [System.Management.Automation.ErrorRecord]) {
        winget.exe install -e --id Python.Python.3.11
    } else {
        $pyEx
    }
    pip install conan==1.60.0
}