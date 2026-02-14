# Skyrim Together Tweaks

SKSE companion plugin for Skyrim Together that provides various tweaks and utilities

Nexus Mods page: https://www.nexusmods.com/skyrimspecialedition/mods/135782

## Requirements
* [CMake](https://cmake.org/)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibSSE](https://github.com/powerof3/CommonLibSSE/tree/dev)

## Building

```pwsh
$env:SkyrimPath = "S:\SteamLibrary\steamapps\common\Skyrim Special Edition\Data"
cmake --preset vs2022-windows-vcpkg-ae
cmake --build buildae --config Release
```

## Credits
- [Skyrim Together](https://github.com/tiltedphoques/TiltedEvolution) for the legendary co-op mod
- [powerof3](https://github.com/powerof3) for the SKSE plugin template
- **xSE RE** community for their reverse engineering efforts

## License
[MIT](LICENSE)
