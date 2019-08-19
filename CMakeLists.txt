cmake_minimum_required(VERSION 3.0.2)
project(svg-cpp-plot)

###################################################################################
# COMPILER FLAGS
###################################################################################

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# Select flags.
if ( ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") )
    set(CMAKE_CXX_FLAGS         "-std=c++17 -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -mtune=native")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
       set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Dsrandom=srand -Drandom=rand -D_USE_MATH_DEFINES")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message("Using Visual Studio, are you sure?")
endif()

message(STATUS "Compiler  = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "System    = ${CMAKE_SYSTEM_NAME}")
message(STATUS "Prefix    = ${CMAKE_PREFIX_PATH}")
message(STATUS "Flags     = ${CMAKE_CXX_FLAGS}")
message(STATUS "Build     = ${CMAKE_BUILD_TYPE}")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
   message(STATUS "R.Flags   = ${CMAKE_CXX_FLAGS_RELEASE}")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
   message(STATUS "D.Flags   = ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   message(STATUS "D.Flags   = ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

##################################################################################
# PATHS
##################################################################################

# Path for output executable files
IF(NOT (IS_DIRECTORY "${svg-cpp-plot_SOURCE_DIR}/bin"))
  file(MAKE_DIRECTORY "${svg-cpp-plot_SOURCE_DIR}/bin")
endif()
set(EXECUTABLE_OUTPUT_PATH "${svg-cpp-plot_SOURCE_DIR}/bin")

# Base include directory
include_directories(${svg-cpp-plot_SOURCE_DIR})

##################################################################################
# LIBPNG
##################################################################################
find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIR})
	list(APPEND svg_cpp_plot_libs ${PNG_LIBRARY})
	list(APPEND svg_cpp_plot_defs "USE_PNG")
endif(PNG_FOUND)

###################################################################################
# USEFUL FUNCTIONS
###################################################################################
function(add_subdirectories_and_compile directory)
	file(GLOB children ${directory}/*)
	get_filename_component(dir ${directory} NAME)
	foreach(fullchild ${children})
		get_filename_component(child ${fullchild} NAME)
		if(IS_DIRECTORY ${fullchild})
			if (EXISTS ${fullchild}/${child}.cc)
				add_executable(svg-${dir}-${child} ${fullchild}/${child}.cc)
				target_compile_definitions(svg-${dir}-${child} PRIVATE ${svg_cpp_plot_defs})
				target_link_libraries(svg-${dir}-${child} ${svg_cpp_plot_libs})
			else()
				add_subdirectory(${fullchild})
			endif()
		endif()
	endforeach()
endfunction(add_subdirectories_and_compile)

###################################################################################
# TARGETS
###################################################################################

add_subdirectories_and_compile(main/examples)
add_subdirectories_and_compile(main/doc)
add_subdirectories_and_compile(main/test)



