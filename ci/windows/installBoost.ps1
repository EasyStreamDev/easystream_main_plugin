function installBoost {
    param(
        [Parameter(Mandatory)]
        [string]$BoostFolder
    )
    if (-not(Test-Path -Path "compileResource")) {
        Write-Host "Creating folder for boost installation"
        mkdir "compileResource"
    }
    Set-Location "compileResource"
    Write-Host "Downloading boost zip file"
    if (Test-Path -Path "boostSource") {
        Remove-Item -Path "boostSource\*" -Recurse
    } else {
        mkdir "boostSource"
    }
    $Url = "https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.zip"
    $Destination = "Boost.zip"
    $ExtractPath = "boostSource"
    Invoke-WebRequest -Uri $Url -OutFile $Destination
    Write-Host "Extracting " + $Destination
    Expand-Archive -LiteralPath $Destination -DestinationPath $ExtractPath
    Remove-Item $Destination -Force
    Set-Location -Path $ExtractPath
    Set-Location "boost_1_81_0"
    .\bootstrap.bat
    .\b2.exe --prefix=$BoostFolder  --build-dir=$BoostFolder threading=multi <#variant=release#> install
}