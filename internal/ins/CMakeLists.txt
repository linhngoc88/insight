# Insight PRIVATE DEPENDENCIES

unset(INSIGHT_PRIVATE_DEPENDENCIES)
unset(INSIGHT_PRIVATE_DEPENDENCIES_INCLUDE_DIRS)

if (INSIGHT_BLAS_LIBRARIES AND INSIGHT_BLAS_INCLUDE_DIRS)
  list(APPEND INSIGHT_PRIVATE_DEPENDENCIES ${INSIGHT_BLAS_LIBRARIES})
  list(APPEND INSIGHT_PRIVATE_DEPENDENCIES_INCLUDE_DIRS
    ${INSIGHT_BLAS_INCLUDE_DIRS})
endif()

# List all internal source files. Do NOT use file(GLOB *) to find source!
set(INSIGHT_SRCS
  errno.c
  block/init.c
  vector/init.c
  vector/oper.c)

# Depend on private header files so that they appear in IDEs.
file(GLOB INSIGHT_INTERNAL_HDRS
  *.h
  block/*.h
  vector/*.h)

# Also depend on public header files so that they appear in IDEs.
file(GLOB INSIGHT_PUBLIC_HDRS
  ${Insight_SOURCE_DIR}/include/ins/*.h
  ${Insight_SOURCE_DIR}/include/ins/block/*.h
  ${Insight_SOURCE_DIR}/include/ins/vector/*.h
  ${Insight_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ins/internal/*.h)

set(INSIGHT_LIBRARY_SOURCE
  ${INSIGHT_SRCS}
  ${INSIGHT_INTERNAL_HDRS}
  ${INSIGHT_PUBLIC_HDRS})

add_library(insight ${INSIGHT_LIBRARY_SOURCE})

target_link_libraries(insight
  PRIVATE ${INSIGHT_PRIVATE_DEPENDENCIES})

target_include_directories(insight
  BEFORE PUBLIC
  $<BUILD_INTERFACE:${Insight_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE ${Insight_SOURCE_DIR}/internal
  PUBLIC $<BUILD_INTERFACE:${Insight_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Add external dependencies include directories to Insight
target_include_directories(insight PRIVATE
  ${INSIGHT_PRIVATE_DEPENDENCIES_INCLUDE_DIRS})

install(TARGETS insight
  EXPORT  InsightExport
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

include(AppendTargetProperty)

if(BUILD_TESTING)
  macro (INS_TEST DIR NAME)
    add_executable(${NAME}_test ${DIR}/${NAME}_test.c)

    target_include_directories(${NAME}_test PRIVATE
      ${Insight_SOURCE_DIR}/internal/ins
      ${CMocka_INCLUDE_DIRS})

    target_link_libraries(${NAME}_test PRIVATE
      insight
      ${CMocka_LIBRARIES})

    # The output directory for this test.
    # We don't wanrt this test to be in the generic bin folder.
    set(OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${DIR})
    set_target_properties(${NAME}_test
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}
    )

    add_test(NAME ${NAME}_test
      COMMAND ${OUTPUT_DIRECTORY}/${NAME}_test
      --test_srcdir
      ${Insight_SOURCE_DIR}/data)
  endmacro()

  # tests
  ins_test(. errno)
  ins_test(block block_double)
  ins_test(block block_float)
  ins_test(block block_int)
  ins_test(vector vector_double_init)
  ins_test(vector vector_double_oper)
endif()
