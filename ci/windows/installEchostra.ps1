#Requires -RunAsAdministrato
param(
    # Parameter help description
    [Parameter()]
    [Switch]$WithCuda
)


function InstallEchostraCuda {
    $Url = "https://github.com/EasyStreamDev/Echostra/archive/refs/heads/Cuda.zip"
    $DestinationZIP = "Echostra.zip"
    $Destination = "C:\Program Files\obs-studio\obs-plugins\64bit\Echostra"
    if (!(Test-Path -Path $Destination)) {
        mkdir $Destination
    }
    Invoke-WebRequest -Uri $Url -OutFile "${Destination}\${DestinationZip}"
    Expand-Archive -Path $DestinationZIP -DestinationPath "${Destination}\${DestinationZip}"
    Set-Location "${Destination}\Echostra-Cuda"
    pip install --user virtualenv
    virtualenv.exe EasystreamEnv
    ./easystreamEnv/scripts/Activate.ps1
    pip install -r ./requirements_win.txt
    pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
}

function InstallEchostraWithoutCuda {
    $Url = "https://github.com/EasyStreamDev/Echostra/archive/refs/heads/main.zip"
    $DestinationZIP = "Echostra.zip"
    $Destination = "C:\Program Files\obs-studio\obs-plugins\64bit\Echostra"
    if (!(Test-Path -Path $Destination)) {
        mkdir $Destination
    }
    Invoke-WebRequest -Uri $Url -OutFile "${Destination}\${DestinationZip}"
    Expand-Archive -Path "${Destination}\${DestinationZip}" -DestinationPath ${Destination}
    Set-Location "${Destination}\Echostra-main"
    pip install --user virtualenv
    virtualenv.exe EasystreamEnv
    ./EasystreamEnv/scripts/Activate.ps1
    pip install -r ./requirements_win.txt
    pip3 install torch torchvision torchaudio
}

function main {
    If ($WithCuda.IsPresent) {
        InstallEchostraCuda
    } else {
        InstallEchostraWithoutCuda
    }
}

main