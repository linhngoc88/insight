# List all internal source files. Do NOT use file(GLOB *) to find source!
set(INSIGHT_SRCS
  errno.c
  malloc_wrappers.c
  matrix_init.c)

# Depend on the header files so that they appear in IDEs.
file(GLOB INSIGHT_INTERNAL_HDRS *.h)
file(GLOB INSIGHT_PUBLIC_HDRS ${Insight_SOURCE_DIR}/include/insight/*.h)

set(INSIGHT_LIBRARY_SOURCE
  ${INSIGHT_SRCS}
  ${INSIGHT_INTERNAL_HDRS}
  ${INSIGHT_PUBLIC_HDRS})

add_library(insight ${INSIGHT_LIBRARY_SOURCE})

target_include_directories(insight
  PRIVATE ${Insight_SOURCE_DIR}/internal
  PUBLIC $<BUILD_INTERFACE:${Insight_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS insight
  EXPORT  InsightExport
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

include(AppendTargetProperty)

if(BUILD_TESTING)
  macro (INSIGHT_TEST NAME)
    add_executable(${NAME}_test ${NAME}_test.c)

    target_include_directories(${NAME}_test PRIVATE
      ${Insight_SOURCE_DIR}/internal/insight
      ${CMOCKA_INCLUDE_DIR})

    target_link_libraries(${NAME}_test PRIVATE
      insight
      ${CMOCKA_LIBRARIES})

    add_test(NAME ${NAME}_test
      COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}_test
      --test_srcdir
      ${Insight_SOURCE_DIR}/data)
  endmacro()

  # tests
  insight_test(errno)
  insight_test(matrix_int)
endif()
