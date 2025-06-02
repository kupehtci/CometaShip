#!/bin/bash
# Settle for macOS and windows
# Define the OS of compilation by using "-t" flag followed by "macos", "windows" or "linux"

## For the case of MACOS
## Clean previous build
## Generate MakeFile and dependencies with targeted OS as macOS
## Compile the project with debug configuration using `make` command
## Run the compiled binary as DEBUG by default
## If you want to run the release version, change the config to release



# Print the usage of the script
print_usage() {
  echo "[BUILD] "
  echo "Usage: "
  echo "    -t   Indicate the type of the OS to compile for"
  echo "         Valid Options are: "
  echo "               * macos   : compile for macos using premake5 to generate a MakeFile that then is compiled using Make"
  echo "               * windows : compile for windows using premake5.exe binary (Not need to install) to generate a Visual Studio 2022 solution and proyect files with configuration"
}

type=""

while getopts 't:h' flag; do
  case "${flag}" in
    t) type="${OPTARG}" 
        ;;
    h) print_usage
       exit 1 ;; 
    *) print_usage
       exit 1 ;;
  esac
done


if [ "$type" = "" ]; then
    echo "[BUILD] Type has not been specified, use -t flag to indicate a valid OS to compile the project for"
    exit 1
fi

# Windows compilation for Visual Studio 2022
compile_for_windows_vstudio() {
    echo "[BUILD] Compiling a Visual Studio Solution for windows using premake5"
    ./premake5.exe clean
    rm *.sln
    rm *.vcxproj*
    ./premake5.exe vs2022 --os=windows
}

# Build windows .sln using MSBuild
#build_for_windows_msbuild(){
#    # This function need to use the MSBuild.exe file ubicated in the system to run the Solution file of the proyect
#    C:\Windows\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe .\Aura.sln
#}

echo "============ STARTED BUILDING SCRIPT ============"

# MacOS compilation and execution using Make Files
compile_for_macos_gmake () {

    # Check that Premake5 is installed
    if command -v premake5 > /dev/null 2>&1; then
        echo "[BUILD] Premake5 is installed"
    else
        echo "[BUILD] Premake5 is not installed and cannot be compiled without it"
        exit 1;
    fi

    echo "[BUILD] Transcription for MacOS using premake5 to GMake files"
    
    premake5 clean
    premake5 gmake --cc=gcc --os=macosx

    echo "[BUILD] Compiling MakeFiles"
    make config=debug

    echo "[BUILD] Executing the binary"

    #  take into account that needs to be called from here to set the current path from here
    #  If its called from inside the folder, the current path or called' path is going to be taken from the debug folder
    bin/Debug/CometaGL
}


case "${type}" in

    "macos")
        compile_for_macos_gmake
    ;; 
    "windows")
        compile_for_windows_vstudio
        # build_for_windows_msbuild
    ;; 
    *)
        echo "Unknown option for compiling, valid options are [macos, windows] in lowercase"
        exit 1
    ;;
esac
