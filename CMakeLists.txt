cmake_minimum_required(VERSION 3.11)

# Policy CMP0074 is not set: find_package uses <PackageName>_ROOT variables.
# Run "cmake --help-policy CMP0074" for policy details.  Use the cmake_policy
# command to set the policy and suppress this warning.
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project(Insight C)

list(APPEND CMAKE_MODULE_PATH "${Insight_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

# Insight output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Insight_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Insight_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Insight_BINARY_DIR}/lib)

# Set postfixes for generated libraries based on buildtype. For example,
# the generated library will be named as `libinsight-debug.a` if `CMAKE_BUILD_TYPE`
# is set to `Debug`, and it will be `libinsight.a` if `CMAKE_BUILD_TYPE` is set
# to `Release`.
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_DEBUG_POSTFIX "-debug")

enable_testing()

option(BUILD_TESTING "Enable tests." ON)

unset(INSIGHT_COMPILE_OPTIONS)

include(PrettyPrintCMakeList)
include(UpdateCacheVariable)

if(BUILD_TESTING)
  find_package(CMocka REQUIRED)
endif()

# BLAS VENDOR

include(InsightListBLASVendors)

# List of all available BLAS vendors on the current machine that Insight
# supports.
insight_list_blas_vendors(INSIGHT_BLAS_VENDORS)
pretty_print_cmake_list(PRETTY_INSIGHT_BLAS_VENDORS ${INSIGHT_BLAS_VENDORS})
message(STATUS "Available BLAS vendors: ${PRETTY_INSIGHT_BLAS_VENDORS}")

set(INSIGHT_BLAS_VENDOR "${INSIGHT_BLAS_VENDOR}" CACHE STRING
  "Insight BLAS vendor" FORCE)

# Set INSIGHT_BLAS_VENDOR to the default BLAS vendor if not specified.
if (NOT INSIGHT_BLAS_VENDOR)
  # Defaults to the first item in the list of available options.
  list(GET INSIGHT_BLAS_VENDORS 0 DEFAULT_BLAS_VENDOR)
  message(STATUS "No BLAS vendor specified; defaulting to "
    "INSIGHT_BLAS_VENDOR=${DEFAULT_BLAS_VENDOR}")
  update_cache_variable(INSIGHT_BLAS_VENDOR ${DEFAULT_BLAS_VENDOR})
endif()

# Check to make sure that INSIGHT_BLAS_VENDOR which was specified via
# -DINSIGHT_BLAS_VENDOR is indeed a valid option.
list(FIND INSIGHT_BLAS_VENDORS ${INSIGHT_BLAS_VENDOR} BLAS_VENDOR_INDEX)
if (BLAS_VENDOR_INDEX EQUAL -1)
  message(FATAL_ERROR
    "Invalid INSIGHT_BLAS_VENDOR: ${INSIGHT_BLAS_VENDOR}. "
    "Available options are: ${PRETTY_INSIGHT_BLAS_VENDORS}")
endif()

unset(INSIGHT_BLAS_INCLUDE_DIRS)
unset(INSIGHT_BLAS_LIBRARIES)

# Set BLAS library based on the specified INSIGHT_BLAS_VENDOR. Note that
# all variables of the form [BLAS_VENDOR]_INCLUDE_DIRS and
# [BLAS_VENDOR]_LIBRARIES have already been set when we listed all
# BLAS vendors above.
if ("${INSIGHT_BLAS_VENDOR}" STREQUAL "OpenBLAS")
  set(INSIGHT_BLAS_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIRS})
  set(INSIGHT_BLAS_LIBRARIES ${OpenBLAS_LIBRARIES})
  list(APPEND INSIGHT_COMPILE_OPTIONS INSIGHT_USE_OPEN_BLAS)
elseif ("${INSIGHT_BLAS_VENDOR}" STREQUAL "AccelerateBLAS")
  set(INSIGHT_BLAS_INCLUDE_DIRS ${AccelerateBLAS_INCLUDE_DIRS})
  set(INSIGHT_BLAS_LIBRARIES ${AccelerateBLAS_LIBRARIES})
  list(APPEND INSIGHT_COMPILE_OPTIONS INSIGHT_USE_ACCELERATE_BLAS)
else()
  message(FATAL_ERROR "Insight bug: unknown INSIGHT_BLAS_VENDOR: "
    "${INSIGHT_BLAS_VENDOR}")
endif()

message(STATUS "INSIGHT_BLAS_VENDOR: ${INSIGHT_BLAS_VENDOR}")
message(STATUS "INSIGHT_BLAS_INCLUDE_DIRS: ${INSIGHT_BLAS_INCLUDE_DIRS}")
message(STATUS "INSIGHT_BLAS_LIBRARIES: ${INSIGHT_BLAS_LIBRARIES}")

# Change the default build type from Debug to Release, while still
# supporting overriding the build type.
#
# The CACHE STRING logic here and elsewhere is needed to force CMake
# to pay attention to the value of these variables.
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type specified; defaulting to CMAKE_BUILD_TYPE=Release.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
else()
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("\n=================================================================================")
    message("\n-- Build type: Debug. Performance might be affected!")
    message("-- Add -DCMAKE_BUILD_TYPE=Release to the CMake command line to get an optimized build.")
    message("\n=================================================================================")
  endif()
endif()

# Configure the config.h compile options header using the current compile
# options and put the configured header into the Insight build directory.
# Note that the ins/internal subdir in <build>/config where the configured
# config.h is placed is important, because Insight will be built against
# this configured header, it needs to have the same relative include path
# as it would if it were in the source tree (or installed).
list(REMOVE_DUPLICATES INSIGHT_COMPILE_OPTIONS)
include(CreateInsightConfig)
create_insight_config("${INSIGHT_COMPILE_OPTIONS}"
  ${Insight_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ins/internal)

add_subdirectory(internal/ins)

# Setup installation of Insight public headers.

file(GLOB INSIGHT_PUBLIC_ROOT_HDRS ${Insight_SOURCE_DIR}/include/ins/*.h)
install(FILES ${INSIGHT_PUBLIC_ROOT_HDRS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ins)

file(GLOB INSIGHT_PUBLIC_BLOCK_HDRS
  ${Insight_SOURCE_DIR}/include/ins/block/*.h)
install(FILES ${INSIGHT_PUBLIC_BLOCK_HDRS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ins/block)

file(GLOB INSIGHT_PUBLIC_VECTOR_HDRS
  ${Insight_SOURCE_DIR}/include/ins/block/*.h)
install(FILES ${INSIGHT_PUBLIC_VECTOR_HDRS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ins/vector)

# Also setup installation of Insight config.h configured with the current
# build options into the installed headers directory.
install(DIRECTORY ${Insight_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
