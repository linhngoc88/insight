# FindOpenBLAS.cmake - Find OpenBLAS library.
#
# This module defines the following variables:
#
# OpenBLAS_FOUND: TRUE iff OpenBLAS has been found.
# OpenBLAS_INCLUDE_DIRS: Include directories for OpenBLAS.
# OpenBLAS_LIBRARIES: Libraries for OpenBLAS and all dependencies.
#
# The following variables are also defined by this module, but in line with
# CMake recommended FindPackage() module style should NOT be referenced
# directly by callers (use the plural variables detailed above instead).
#
# OpenBLAS_INCLUDE_DIR: Include directory for OpenBLAS, not including the
#                       include directory of any dependencies.
# OpenBLAS_LIBRARY: OpenBLAS library, not including the libraries of any
#                   dependencies.

SET(OpenBLAS_INCLUDE_SEARCH_PATHS
  /usr/include
  /usr/include/openblas
  /usr/include/openblas-base
  /usr/local/include
  /usr/local/include/openblas
  /usr/local/include/openblas-base

  # OpenBLAS was installed via homebrew on Mac
  /usr/local/opt/openblas/include
  /opt/homebrew/opt/openblas/include

  /opt/OpenBLAS/include
  $ENV{OpenBLAS_HOME}
  $ENV{OpenBLAS_HOME}/include
)

SET(OpenBLAS_LIB_SEARCH_PATHS
  /lib/
  /lib/openblas-base
  /lib64/
  /usr/lib
  /usr/lib/openblas-base
  /usr/lib64
  /usr/local/lib
  /usr/local/lib64
  /opt/OpenBLAS/lib

  # OpenBLAS was installed via homebrew on Mac
  /usr/local/opt/openblas/lib
  /opt/homebrew/opt/openblas/lib

  $ENV{OpenBLAS}cd
  $ENV{OpenBLAS}/lib
  $ENV{OpenBLAS_HOME}
  $ENV{OpenBLAS_HOME}/lib
)

unset(OpenBLAS_FOUND)

find_path(OpenBLAS_INCLUDE_DIR
  NAMES cblas.h
  PATHS ${OpenBLAS_INCLUDE_SEARCH_PATHS})

find_library(OpenBLAS_LIBRARY
  NAMES openblas
  PATHS ${OpenBLAS_LIB_SEARCH_PATHS})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(OpenBLAS
  FOUND_VAR OpenBLAS_FOUND
  REQUIRED_VARS OpenBLAS_LIBRARY OpenBLAS_INCLUDE_DIR)

if (OpenBLAS_FOUND)
  set(OpenBLAS_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIR})
  set(OpenBLAS_LIBRARIES ${OpenBLAS_LIBRARY})
  mark_as_advanced(FORCE OpenBLAS_INCLUDE_DIR OpenBLAS_LIBRARY)
endif()
