# The config file
set(INSIGHT_CONFIG_IN_FILE "${CMAKE_CURRENT_LIST_DIR}/config.h.in")

# CreateInsightConfig.cmake - Create the config.h for Insight.
#
# This function configures the Insight config.h file based on the current
# compile options and copies it into the specified location. It should be
# called before Insight is built so that the correct config.h is used when
# Insight is compiled.
#
# INPUTS:
#
#   - CURRENT_INSIGHT_COMPILE_OPTIONS - A list of currently enabled Insight
#     compile options. These are compared against the full list of valid
#     options, which are read from config.h.in. Any options which are not
#     part of the valid set will invoke an error. Any valid option present
#     will be enabled in the resulting config.h. All other options will be
#     disabled.
#
#   - INSIGHT_CONFIG_OUTPUT_DIRECTORY - The path to output directory in
#     which to save to configured file config.h. Typically, this will be
#     <src>/include/ins/internal.

function(create_insight_config
    CURRENT_INSIGHT_COMPILE_OPTIONS
    INSIGHT_CONFIG_OUTPUT_DIRECTORY)
  # Create the specified output directory if it does not exist.
  if (NOT EXISTS "${INSIGHT_CONFIG_OUTPUT_DIRECTORY}")
    message(STATUS "Creating the output directory for config.h: "
      "${INSIGHT_CONFIG_OUTPUT_DIRECTORY}")
    file(MAKE_DIRECTORY "${INSIGHT_CONFIG_OUTPUT_DIRECTORY}")
  endif()

  # Check to make sure that the specified INSIGHT_CONFIG_OUTPUT_DIRECTORY
  # is indeed a valid directory.
  if (NOT IS_DIRECTORY "${INSIGHT_CONFIG_OUTPUT_DIRECTORY}")
    message(FATAL_ERROR "The specified INSIGHT_CONFIG_OUTPUT_DIRECTORY: "
      "${INSIGHT_CONFIG_OUTPUT_DIRECTORY} exists, but it is not a directory")
  endif()

  # Read all possible configurable compile options from config.h.i, this
  # avoids us from having to hard-code in this file what the valid options
  # are.
  file(READ ${INSIGHT_CONFIG_IN_FILE} INSIGHT_CONFIG_IN_CONTENT)
  string(REGEX MATCHALL "@[^@ $]+@"
    ALL_CONFIGURABLE_INSIGHT_OPTIONS "${INSIGHT_CONFIG_IN_CONTENT}")
  # Removing @ symbol at the beginning and end of each option.
  string(REPLACE "@" "" ALL_CONFIGURABLE_INSIGHT_OPTIONS
    "${ALL_CONFIGURABLE_INSIGHT_OPTIONS}")

  # Ensure that there are no repetitions in the current compile options.
  list(REMOVE_DUPLICATES CURRENT_INSIGHT_COMPILE_OPTIONS)

  foreach (INSIGHT_OPTION ${ALL_CONFIGURABLE_INSIGHT_OPTIONS})
    # Try and find the option in the list of current compile options,
    # if it is present, then the option is enabled, otherwise it is disabled.
    list(FIND CURRENT_INSIGHT_COMPILE_OPTIONS ${INSIGHT_OPTION}
      OPTION_ENABLED)

    # list(FIND ..) returns -1 if the element was not in the list, but CMake
    # interprets if (VAR) to be true if VAR is any none-zero number, even
    # negative ones, hence we have to explicitly check for >= 0.
    if (OPTION_ENABLED GREATER -1)
      message(STATUS "Enabling ${INSIGHT_OPTION} in config.h")
      set(${INSIGHT_OPTION} "#define ${INSIGHT_OPTION}")

      # Remove the item from the list of current options so that we can
      # identify any options that were in CURRENT_INSIGHT_COMPILE_OPTIONS,
      # but not in ALL_CONFIGURABLE_INSIGHT_OPTIONS (which is an error).
      list(REMOVE_ITEM CURRENT_INSIGHT_COMPILE_OPTIONS ${INSIGHT_OPTION})
    else()
      set(${INSIGHT_OPTION} "// #define ${INSIGHT_OPTION}")
    endif()
  endforeach()

  # CURRENT_INSIGHT_COMPILE_OPTIONS should now be an empty list, any elements
  # remaining were not present in ALL_CONFIGURABLE_INSIGHT_OPTIONS read from
  # config.h.in
  if (CURRENT_INSIGHT_COMPILE_OPTIONS)
    message(FATAL_ERROR "CURRENT_INSIGHT_COMPILE_OPTIONS "
      "contains the following options which were not present in "
      "config.h.in: ${CURRENT_INSIGHT_COMPILE_OPTIONS}")
  endif()

  configure_file(${INSIGHT_CONFIG_IN_FILE}
    "${INSIGHT_CONFIG_OUTPUT_DIRECTORY}/config.h" @ONLY)
endfunction()
