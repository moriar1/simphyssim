# Preview
> TODO

# Build
## Option 1. CMake with FetchContent (recomended)
Prerequisite:
- git (on Windows must be available in terminal, i.e. path to the git folder is in the PATH system variable)
- CMake

Run:
```Shell
git clone https://codeberg.org/ulfecoder/simple-physical-simulations
cd simple-physical-simulations
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make
./simulation
```

## Option 2. VS 2022 + MSBuild with vcpkg
1) Create new C++ Empty Project.
2) Move all .hpp files in include folder to headers of project.
3) Move all .cpp files in src folder to sources of project.
4) Open the Developer command prompt in Visual Studio.
    - Menu: View -> Terminal or Menu: Tools -> Command Line -> Developer PowerShell
5) Run:
```PowerShell
vcpkg integrate install
vcpkg new --application
```
6) Enable manifest mode.
    - Navigate to the Project Properties pages of project. Under Configuration Properties > vcpkg, set Use vcpkg manifest to Yes.
7) Run:
```PowerShell
vcpkg add port imgui-sfml
vcpkg add port implot
```
8) Build.

More informaion: https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-msbuild?pivots=shell-cmdon

# Used libraries
- [SFML](https://github.com/SFML/SFML)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [ImGui-SFML](https://github.com/SFML/imgui-sfml)
- [ImPlot](https://github.com/epezent/implot)

# Tips
- Press ctrl and click on slider line to enter exact value
- Right click on table to hide colomns.
- In the "Data" window you could see Standard deviation of arithmetic mean of full mechanical energy. If set Friction coefficient to 0.0 it will show the calculating error of mechanical energy. So, for example, if you have 1000000 values in container (shows lower), you will be able to multiply it on 1.959966 (Student's t-distribution for 95%) to get relative error.