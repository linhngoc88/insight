# FindCMocka.cmake - Find the cmocka library (https://cmocka.org/).
#
# This module defines the following variables:
#
# CMocka_FOUND:        TRUE iff cmocka library has been found.
# CMocka_INCLUDE_DIRS: Include directories for cmocka.
# CMocka_LIBRARIES:    Libraries required to link cmocka.
#
# The following variables are also defined by this module, but in line with
# CMake recommended FindPackage() module style should NOT be referenced
# directly by callers (use the plural variables detailed above instead).
#
# CMocka_INCLUDE_DIR: Include directory for cmocka, not including the
#                     include directory of any dependencies.
# CMocka_LIBRARY:     cmocka library, not including the libraries of
#                     any dependencies.

unset(CMocka_FOUND)

find_path(CMocka_INCLUDE_DIR NAMES cmocka.h)
find_library(CMocka_LIBRARY NAMES cmocka)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CMocka
  FOUND_VAR CMocka_FOUND
  REQUIRED_VARS CMocka_LIBRARY CMocka_INCLUDE_DIR)

if (CMocka_FOUND)
  set(CMocka_INCLUDE_DIRS ${CMocka_INCLUDE_DIR})
  set(CMocka_LIBRARIES ${CMocka_LIBRARY})
  mark_as_advanced(FORCE CMocka_INCLUDE_DIR CMocka_LIBRARY)
endif()
