##---------------------------------------------------------------------------##
## CMakeLists.txt
## Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
##---------------------------------------------------------------------------##

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# Define the project and set project name
project(YAYP
  VERSION 0.0.1
  DESCRIPTION "Yet Another YAML Parser (YAYP)"
  LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add project cmake modules to module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# SETUP BUILD OPTIONS
# By default, we turn on debug builds, and enable Design-By-Contract
# For release builds, set CMAKE_BUILD_TYPE to "Release" and set
# YAYP_DBC to 0
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Type of build")
option(YAYP_BUILD_DOC "Turn on/off in-code documentation" ON)
option(YAYP_ENABLE_TESTS "Enable unit tests" ON)
set(YAYP_DBC 3 CACHE STRING "Set Design-By-Contract assertion level.
  0: All design-by-contract macros disabled,
  1: Enables YAYP_REQUIRE()
  2: Enables YAYP_REQUIRE(), YAYP_REMEMBER(), and YAYP_ENSURE()
  3: Enables all DBC")

# SETUP DOXYGEN DOCUMENTATION
if (YAYP_BUILD_DOC)
  include(AddDoxygenTarget)
  add_doxygen_target(doxygen SOURCEDIR ${CMAKE_CURRENT_SOURCE_DIR})
else ()
  message(STATUS "Doxygen documentation disabled!")
endif ()

# SETUP GOOGLETEST UNIT TESTING FRAMEWORK
if (YAYP_ENABLE_TESTS)
  find_package(GTest REQUIRED)
  include(CTest)
  enable_testing()
else ()
  message("Unit tests disabled!")
endif ()

# Report YAYP DBC and Timing settings
message(STATUS "YAYP DBC set to " ${YAYP_DBC})
add_definitions("-DYAYP_DBC=${YAYP_DBC}")

# Add all code
list(APPEND HEADERS
  src/harness/DBC.hh
  src/harness/Macros.hh
  src/harness/SoftEqual.hh
  src/harness/SoftEqual.i.hh
  src/harness/Testing.hh
  src/harness/Testing.i.hh
  src/harness/detail/TestingFunctions.hh
  src/harness/detail/TestingFunctions.i.hh
  src/core/FileFunctions.hh
  src/core/StringFunctions.hh
  src/core/StringFunctions.i.hh
  )
list(APPEND SOURCES
  src/harness/DBC.cc
  src/core/FileFunctions.cc
  src/core/StringFunctions.cc
  )

# Build and install library
add_library(yayp ${SOURCES})
target_include_directories(yayp PUBLIC ${PROJECT_SOURCE_DIR}/src)
install(TARGETS yayp LIBRARY)

# Install headers
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

# Build tests
if (YAYP_ENABLE_TESTS)
  add_subdirectory(src/harness/tests)
  add_subdirectory(src/harness/detail/tests)
  add_subdirectory(src/core/tests)
endif ()


##---------------------------------------------------------------------------##
## end of CMakeLists.txt
##---------------------------------------------------------------------------##
