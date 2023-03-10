# Append item(s) to a property on a declared CMake target:
#
#    append_target_property(target property item_to_append1
#                                           [... item_to_appendN])
#
# The set_target_properties() CMake function will overwrite the contents of the
# specified target property.  This function instead appends to it, so can
# be called multiple times with the same target & property to iteratively
# populate it.
function(append_target_property TARGET PROPERTY)
  if (NOT TARGET ${TARGET})
    message(FATAL_ERROR "Invalid target: ${TARGET} cannot append: ${ARGN} "
      "to property: ${PROPERTY}")
  endif()
  if (NOT PROPERTY)
    message(FATAL_ERROR "Invalid property to update for target: ${TARGET}")
  endif()
  # Get the initial state of the specified property for the target s/t
  # we can append to it (not overwrite it).
  get_target_property(INITIAL_PROPERTY_STATE ${TARGET} ${PROPERTY})
  if (NOT INITIAL_PROPERTY_STATE)
    # Ensure that if the state is unset, we do not insert the XXX-NOTFOUND
    # returned by CMake into the property.
    set(INITIAL_PROPERTY_STATE "")
  endif()
  # Delistify (remove ; separators) the potentially set of items to append
  # to the specified target property.
  string(REPLACE ";" " " ITEMS_TO_APPEND "${ARGN}")
  set_target_properties(${TARGET} PROPERTIES ${PROPERTY}
    "${INITIAL_PROPERTY_STATE} ${ITEMS_TO_APPEND}")
endfunction()
