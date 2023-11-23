#Requires -RunAsAdministrato
param(
    [Parameter()]
    [Switch]$Check
)

function checkWindows10 {
    return (Get-WmiObject Win32_OperatingSystem).Caption -Match "Windows 10"
}

function checkWindows11 {
    (Get-WmiObject Win32_OperatingSystem).Caption -Match "Windows 11"
}

function checkIfCudaInstalled {
    nvcc --version
    if ($?) {
        return $true
    }
    return $false
}

function checkIfCudaAvailable {
    $gpuInfo = nvidia-smi -q | Out-String

    if ($gpuInfo -match "NVIDIA") {
        # Check if CUDA is available
        if ($gpuInfo -match "CUDA Version") {
            # $cudaVersion = $matches[0]
            Write-Host "CUDA is available."
            return $true
        }
        else {
            Write-Host "CUDA is not available on this GPU."
        }
    }
    else {
        Write-Host "NVIDIA GPU is not installed or not detected."
    }
    return $false
}


function main
{
    # $var = 1
    # while ($var -eq 1) {
    #     Write-Host "Test"
    # }

    if ($Check.IsPresent) {
        if (!(checkIfCudaAvailable)) {
            # Start-Sleep -Seconds 10
            exit 0
        }
    
        if (checkIfCudaInstalled) {
            Write-Host "Cuda is already installed, aborting"
            # Start-Sleep -Seconds 10
            exit 1
        }
        # Start-Sleep -Seconds 10
        exit 2
    } else {
        if (checkWindows11) {
            $Url = "https://developer.download.nvidia.com/compute/cuda/12.2.2/local_installers/cuda_12.2.2_537.13_windows.exe"
            $Destination = "CudaSetup.exe"
            Invoke-WebRequest -Uri $Url -OutFile $Destination
            .\CudaSetup.exe
        } elseif (checkWindows10) {
            $Url = "https://developer.download.nvidia.com/compute/cuda/12.2.2/local_installers/cuda_12.2.2_537.13_windows.exe"
            $Destination = "CudaSetup.exe"
            Invoke-WebRequest -Uri $Url -OutFile $Destination
            .\CudaSetup.exe
        } else {
            Write-Host "Cannot detect windows version, aborting"
        }
    }

}

main