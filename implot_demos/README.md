# ImPlot Demos

This repository contains toy [ImPlot](https://github.com/epezent/implot) applications that demonstrate some of the library's functionality. The demos here are more sophisticated than those provided in `implot_demo.h`, and may rely on third-party libraries that would otherwise clutter ImPlot's main repository. Use these examples as a starting point for your application and to familiarize yourself with ImPlot, but do not assume all code (e.g. signal processing algorithms) to be perfectly correct or optimal.

## Requirements

- OpenGL
- C++17 compiler

**Note**: Compiled and tested with MSVC. Minor changes to `CMakeLists.txt` may be required to support other compilers. If you make a fix, please submit a PR.

## Build Instructions
1. Clone this repository, [ImPlot](https://github.com/epezent/implot), and [ImGui](https://github.com/ocornut/imgui) into a parent directory (name doesn't matter):

```shell
mkdir root
cd root
git clone https://github.com/epezent/implot_demos
git clone https://github.com/epezent/implot
git clone https://github.com/ocornut/imgui
```

- `root/`
    - `imgui/`
    - `implot/`
    - `implot_demos/`
2. Build with CMake, e.g.:
```shell
cd implot_demos
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Demos

|Demo|Description|Image|
|---|---|---|
|`demo.cpp`|Main demo. Displays the ImPlot (and ImGui) library supplied demo windows.| |
|`filter.cpp`|Simple filter toy for educational purposes. Displays time domain input/output signals, and the frequency domain transfer function, amplitude spectrum, etc.|![filter](https://raw.githubusercontent.com/epezent/implot_demos/master/screenshots/filter.png)|
|`spectrogram.cpp`|Realtime audio spectrogram and visualizer with device playback. Pass in an audio file at the command line: `./spectrogram.exe ./aphex_twin_formula.wav` (see `resources/audio` folder). *Recommend compiling with ImPlot `backends` branch.*|![spectrogram](https://github.com/epezent/implot_demos/blob/master/screenshots/spectrogram.png)|
|`maps.cpp`|OpenStreetMap world map viewer. Downloads and displays zoomable tile maps in a plot.|![maps](https://github.com/epezent/implot_demos/blob/master/screenshots/maps.png)|
|`mandel.cpp`|Realtime Mandelbrot viewer using AVX2 and OpenMP acceleration. *Recommend compiling with ImPlot `backends` branch.*|![mandel](https://github.com/epezent/implot_demos/blob/master/screenshots/mandel.png)|
|`perlin.cpp`|Renders perlin noise in a heatmap.|![perlin](https://github.com/epezent/implot_demos/blob/master/screenshots/perlin.png)|
|`graph.cpp`|Simple graphing calculator using `exprtk` expression evaluator.|![perlin](https://github.com/epezent/implot_demos/blob/master/screenshots/graph.png)|
|`stocks.cpp`|Downloads and displays historical stock data from Yahoo Finance.|![perlin](https://github.com/epezent/implot_demos/blob/master/screenshots/stocks.png)|

## Contributing

Have you created something awesome with ImPlot? If so, please consider submitting a PR with a minimal working example of your application as a demo. Use the following guidelines:

- Modify your ImPlot/ImGui code so that it subclasses `common/App.h` (see any existing demos).
- Try to contain your demo to a single file located in `demos/`.
- Indicate your authorship and any helpful documentation at the top of your demo source file.
- Add dependencies with CMake `FetchContent` (preferred), git submodules, or manually to the `3rdparty/` folder (include the license).
- If your demo requires resources (audio, images, etc.), add them to the `resources/` folder. Keep it simple!
- Screenshots and/or GIFs with your PR are helpful.
