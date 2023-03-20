# FindAccelerateBLAS.cmake - Find Apple Accelerate framework.
#
# This module defines the following variables:
#
# AccelerateBLAS_FOUND: TRUE iff Accelerate framework has been found.
# AccelerateBLAS_INCLUDE_DIRS: Include directories for Accelerate framework.
# AccelerateBLAS_LIBRARIES: Libraries for Accelerate framework and all
#                           dependencies.
#
# The following variables are also defined by this module, but in line with
# CMake recommended FindPackage() module style should NOT be referenced
# directly by callers (use the plural variables detailed above instead).
#
# AccelerateBLAS_INCLUDE_DIR: Include directory for cmocka, not including
#                             the include directory of any dependencies.
# AccelerateBLAS_LIBRARY: Accelerate framework, not including the libraries
#                         of any dependencies.

unset(AccelerateBLAS_FOUND)

find_path(AccelerateBLAS_INCLUDE_DIR NAMES Accelerate.h)
find_library(AccelerateBLAS_LIBRARY NAMES Accelerate)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(AccelerateBLAS
  FOUND_VAR AccelerateBLAS_FOUND
  REQUIRED_VARS AccelerateBLAS_LIBRARY AccelerateBLAS_INCLUDE_DIR)

if (AccelerateBLAS_FOUND)
  set(AccelerateBLAS_INCLUDE_DIRS ${AccelerateBLAS_INCLUDE_DIR})
  set(AccelerateBLAS_LIBRARIES ${AccelerateBLAS_LIBRARY})
  mark_as_advanced(FORCE AccelerateBLAS_INCLUDE_DIR AccelerateBLAS_LIBRARY)
endif()
