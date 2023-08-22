# AssaultCubeHack
Learning to hack via games using DLL Injection.

Using a classic DLL injection technique described [here](https://cocomelonc.github.io/tutorial/2021/09/20/malware-injection-2.html).

Offsets were found using [Cheat Engine](https://www.cheatengine.org/).

> Beware, installation of cheat engine also comes with a pesky anti-virus software I uninstalled.

### Dependencies

To use [`imgui`](https://github.com/ocornut/imgui) and specifically the `docking` branch follow `vcpkg`'s [getting started](https://vcpkg.io/en/getting-started) page. Once installed, ensure the optional step: *Using vcpkg with MSBuild / Visual Studio (may require elevation)* is completed. Open a terminal in the same directory as your installation and run `vcpkg install`. The project uses `vcpkg`'s manifest mode so the *vcpkg.json* will be read leading to imgui's installation.

> My `vcpkg` installation resides within my repository's root directory.

With the core `imgui` installed, next acquire the `imgui` bindings to `DirectX11` and `win32` by downloading them from [here](https://github.com/ocornut/imgui/tree/docking/backends).

Files to download:

1. *imgui_impl_win32.h*
2. *imgui_impl_win32.cpp*
3. *imgui_impl_dx11.h*
4. *imgui_impl_dx11.cpp*

Copy these into the projects source directory and add them to the project.

Lastly, open the project's `Properties > Linker > Input` panel and add the following to the *Addition Dependencies* option.

1. d3d11.lib
2. d3dcompiler.lib

> Items are deliminated by a ";".