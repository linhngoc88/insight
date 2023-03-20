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

# Set postfixes for generated libraries based on buildtype.
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_DEBUG_POSTFIX "-debug")

# Testing

enable_testing()

option(BUILD_TESTING "Enable tests." ON)

if(BUILD_TESTING)
  find_package(CMocka REQUIRED)
endif()

add_subdirectory(internal/ins)

# Install Insight public headers.

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
