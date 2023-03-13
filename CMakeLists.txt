cmake_minimum_required(VERSION 3.11)

# Policy CMP0074 is not set: find_package uses <PackageName>_ROOT variables.
# Run "cmake --help-policy CMP0074" for policy details.  Use the cmake_policy
# command to set the policy and suppress this warning.
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project(Insight C)
list(APPEND CMAKE_MODULE_PATH "${Insight_SOURCE_DIR}/cmake")

# Insight output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Insight_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Insight_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Insight_BINARY_DIR}/lib)

# Testing

enable_testing()
option(BUILD_TESTING "Enable tests." ON)

if(BUILD_TESTING)
  find_package(CMocka QUIET)
  if (NOT CMOCKA_FOUND)
    message(FATAL_ERROR "Cmocka (https://cmocka.org/) not found!")
  endif()
endif()

add_subdirectory(internal/ins)

# Setup installation of Insight public headers.
file(GLOB INSIGHT_HDRS ${Insight_SOURCE_DIR}/include/ins/*.h)
install(FILES ${INSIGHT_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ins)
