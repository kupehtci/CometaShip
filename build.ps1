# Script to build Cometa project for Windows
# Usage: .\build.ps1 -Type [macos|windows]

param(
    [Parameter(Mandatory=$false)]
    [string]$Type
)

function Print-Usage {
    Write-Host "[BUILD] "
    Write-Host "Usage: "
    Write-Host "    -Type   Indicate the type of the OS to compile for"
    Write-Host "         Valid Options are: "
    Write-Host "               * macos   : compile for macos using premake5 to generate a MakeFile"
    Write-Host "               * windows : compile for windows using premake5.exe to generate VS2022 solution"
}

function Compile-WindowsVStudio {
    Write-Host "[BUILD] Compiling a Visual Studio Solution for windows using premake5"
    
    # Clean previous build files
    if (Test-Path "*.sln") {
        Remove-Item "*.sln"
    }
    if (Test-Path "*.vcxproj*") {
        Remove-Item "*.vcxproj*"
    }
    
    # Generate Visual Studio 2022 solution
    .\premake5.exe clean
    .\premake5.exe vs2022 --os=windows

    # Build the solution using MSBuild
    Write-Host "[BUILD] Building the solution with MSBuild"
    $msbuildPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    
    if (Test-Path $msbuildPath) {
        & $msbuildPath .\CometaGL.sln /p:Configuration=Debug
        
        # Check if build was successful
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[BUILD] Build successful"
            Write-Host "[BUILD] Running the binary"
            .\bin\Debug\CometaGL.exe
        } else {
            Write-Host "[BUILD] Build failed with exit code $LASTEXITCODE"
            exit 1
        }
    } else {
        Write-Host "[BUILD] MSBuild not found at expected location. Please ensure Visual Studio 2022 is installed."
        exit 1
    }
}

function Compile-MacOSGMake {
    Write-Host "[BUILD] MacOS compilation is not supported on Windows PowerShell"
    exit 1
}

# Main script execution
Write-Host "============ STARTED BUILDING SCRIPT ============"

if ([string]::IsNullOrEmpty($Type)) {
    Write-Host "[BUILD] Type has not been specified, use -Type parameter to indicate a valid OS"
    Print-Usage
    exit 1
}

switch ($Type.ToLower()) {
    "windows" {
        Compile-WindowsVStudio
    }
    "macos" {
        Compile-MacOSGMake
    }
    default {
        Write-Host "Unknown option for compiling, valid options are [macos, windows] in lowercase"
        exit 1
    }
}
