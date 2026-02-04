cmake_minimum_required(VERSION 3.22)
project(stl LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(STL_SKIP_CONDA_CHECK "Skip enforcement of the active Conda environment" OFF)
set(STL_EXPECTED_CONDA_ENV "stl" CACHE STRING "Conda environment required to build this project")

if(NOT STL_SKIP_CONDA_CHECK)
    if(NOT DEFINED ENV{CONDA_DEFAULT_ENV})
        message(FATAL_ERROR "Please activate the '${STL_EXPECTED_CONDA_ENV}' Conda environment or set STL_SKIP_CONDA_CHECK=ON")
    elseif(NOT "$ENV{CONDA_DEFAULT_ENV}" STREQUAL "${STL_EXPECTED_CONDA_ENV}")
        message(FATAL_ERROR
                "Detected Conda environment '$ENV{CONDA_DEFAULT_ENV}', expected '${STL_EXPECTED_CONDA_ENV}'. "
                "Activate the correct env or set STL_SKIP_CONDA_CHECK=ON if you know what you're doing.")
    endif()
endif()

include(CTest)
enable_testing()

add_library(stl_hello src/hello.cpp)
target_include_directories(stl_hello PUBLIC include)
target_compile_features(stl_hello PUBLIC cxx_std_23)

add_executable(stl_hello_tests tests/hello_tests.cpp)
target_link_libraries(stl_hello_tests PRIVATE stl_hello)
add_test(NAME stl_hello_returns_hello_world COMMAND stl_hello_tests)
