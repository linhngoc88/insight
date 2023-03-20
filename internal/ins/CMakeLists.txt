# List all internal source files. Do NOT use file(GLOB *) to find source!
set(INSIGHT_SRCS
  errno.c
  block/init.c
  vector/init.c)

# Depend on the private header files so that they appear in IDEs.
file(GLOB INSIGHT_INTERNAL_HDRS
  *.h
  block/*.h
  vector/*.h)

# Alos depend on the private header files so that they appear in IDEs.
file(GLOB INSIGHT_PUBLIC_HDRS
  ${Insight_SOURCE_DIR}/include/ins/*.h
  ${Insight_SOURCE_DIR}/include/ins/block/*.h
  ${Insight_SOURCE_DIR}/include/ins/vector/*.h)

set(INSIGHT_LIBRARY_SOURCE
  ${INSIGHT_SRCS}
  ${INSIGHT_INTERNAL_HDRS}
  ${INSIGHT_PUBLIC_HDRS})

add_library(ins ${INSIGHT_LIBRARY_SOURCE})

target_include_directories(ins
  PRIVATE ${Insight_SOURCE_DIR}/internal
  PUBLIC $<BUILD_INTERFACE:${Insight_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ins
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
      ${CMOCKA_INCLUDE_DIR})

    target_link_libraries(${NAME}_test PRIVATE
      ins
      ${CMOCKA_LIBRARIES})

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
endif()
