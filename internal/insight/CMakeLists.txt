# List all internal source files. Do NOT use file(GLOB *) to find source!
set(INSIGHT_SOURCE_FILES
  hello.c)

# Depend also on public headers so they appear in IDEs.
file(GLOB INSIGHT_PUBLIC_HEADER_FILES
  # Global public header files
  ${Insight_SOURCE_DIR}/include/insight/*.h)

set(INSIGHT_LIBRARY_SOURCE
  ${INSIGHT_SOURCE_FILES}
  ${INSIGHT_PUBLIC_HEADER_FILES})

add_library(insight ${INSIGHT_LIBRARY_SOURCE})

target_include_directories(insight PUBLIC
  $<BUILD_INTERFACE:${Insight_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS insight
  EXPORT  InsightExport
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

include(AppendTargetProperty)

if(BUILD_TESTING)
  macro (INSIGHT_TEST NAME)
    add_executable(${NAME}_test ${NAME}_test.c)
    target_include_directories(${NAME}_test PUBLIC ${CMOCKA_INCLUDE_DIR})
    set(OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # We don't want this test to be in the generic bin folder.
    set_target_properties(${NAME}_test
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

    target_link_libraries(${NAME}_test insight ${CMOCKA_LIBRARIES})
    add_test(NAME ${NAME}_test COMMAND ${OUTPUT_DIRECTORY}/${NAME}_test)
  endmacro()

  # tests
  insight_test(hello)
endif()
