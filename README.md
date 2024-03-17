# svg-cpp-plot
A simple C++17 header-only library for generating SVG images and plots. It provides a huge (undocumented) codebase that serves as base for smaller (partially documented) useful APIs and executable files for different specific applications. All this funcitonality has been superseeded by my newer library [svg.cc](https://github.com/adolfomunoz/svg.cc). I won't be mantaining this library but feel free to use it.

The only working application is [SVGPlot](./doc/svgplot.md), an API for plotting different types of graphs in a `matplotlib.pyplot` way with vector (SVG) output.

# Installation

This is a header-only library, so installation is rather simple (it does not require any compilation process). You just need to download
the library in a specific folder:

```
folder> git clone https://github.com/adolfomunoz/svg-cpp-plot.git
```

Then make sure `folder` is within the included directories (`-Ifolder` parameter in g++, `include_directories("folder")` in CMake) and 
include it from C++.

```cpp
#include <svg-cpp-plot/svg-cpp-plot.h>
```

Some features have some external dependencies but they are not mandatory:
- Bicubic interpolation in `imshow` depends on `libpng`. To use it make sure to have `libpng` installed in your system and linked against the executable code, and make sure to define the `USE_PNG` macro, either within your source code by adding `#define USE_PNG` before `#include <svg-cpp-plot/svg-cpp-plot.h>`, by using the commandline  (`-DUSE_PNG` in g++) or in cmake (for your `executable` you would add `target_compile_definitions(executable PRIVATE "USE_PNG"}`).

If your project is already in git, `svg-cpp-plot` can also be included as a submodule with git:

```
git submodule add https://github.com/adolfomunoz/svg-cpp-plot.git
```

If you use CMake as part of the build process, this can also be included as an external project with all the detection of a potentially
installed `libpng` as follows:

```cmake
find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIR})
	list(APPEND svg_cpp_plot_libs ${PNG_LIBRARY})
	list(APPEND svg_cpp_plot_defs "USE_PNG")
endif(PNG_FOUND)

if (NOT EXTERNAL_INSTALL_LOCATION)
	set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)
endif()
if (NOT IS_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
	file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
endif()

include(ExternalProject)
# External include directory
include_directories(${EXTERNAL_INSTALL_LOCATION})
add_custom_target(update)

ExternalProject_Add(svg-cpp-plot
  GIT_REPOSITORY https://github.com/adolfomunoz/svg-cpp-plot.git
  SOURCE_DIR ${EXTERNAL_INSTALL_LOCATION}/svg-cpp-plot
  UPDATE_DISCONNECTED 1
  STEP_TARGETS update
  BUILD_COMMAND ""
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND ""
)
add_dependencies(update svg-cpp-plot-update)
```

and then for your `executable` that uses the library:

```cmake
add_executable(executable ${source_files})
add_dependencies(executable svg-cpp-plot)
target_link_libraries(visualize ${svg_cpp_plot_libs})
target_compile_definitions(visualize PRIVATE ${svg_cpp_plot_defs})
```

This will download `svg-cpp-plot` right before compiling `executable`. If you need to update `svg-cpp-plot` you can 
use the target `update` as in:

```
make update
```

or you can setup `UPDATE_DISCONNECTED 0` so it gets updated every time (but this gets annoying quite soon).


