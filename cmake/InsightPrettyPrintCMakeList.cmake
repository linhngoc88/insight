# insight_pretty_print_cmake_list(OUTPUT_VAR [item1 [item2 ...]])
#
# Sets ${OUTPUT_VAR} in the caller's scope to a human-readable string
# represenation of the list passed as the remaining argument formed
# as: "[item1, item2, ..., itemN]".
function(insight_pretty_print_cmake_list OUTPUT_VAR)
  string(REPLACE ";" ", " PRETTY_LIST_STRINGS "[${ARGN}]")
  set(${OUTPUT_VAR} ${PRETTY_LIST_STRINGS} PARENT_SCOPE)
endfunction()
