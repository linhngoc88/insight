# TODO(linh): add docs to this shit.
function(insight_install_public_headers DIR)
  file(GLOB INSIGHT_HDRS ${Insight_SOURCE_DIR}/include/${DIR}/*.h)
  install(FILES ${INSIGHT_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${DIR})
endfunction()
