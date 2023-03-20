# insight_list_blas_vendors(OUTPUT_VAR)
#
# Sets ${OUTPUT_VAR} in the caller's scrope to a list of available BLAS
# options in the the current system.

# Non-Darwin BLAS options.
set(INSIGHT_BLAS_VENDORS "OpenBLAS")

# Include the BLAS that comes with the Accelerate framework if the current
# system is Darwin.
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  list(APPEND INSIGHT_BLAS_VENDORS "AccelerateBLAS")
endif()

function(insight_list_blas_vendors OUTPUT_VAR)
  set(AVAILABLE_BLAS_VENDORS ${INSIGHT_BLAS_VENDORS})

  # OpenBLAS
  find_package(OpenBLAS QUIET)
  if (NOT OpenBLAS_FOUND)
    list(REMOVE_ITEM AVAILABLE_BLAS_VENDORS "OpenBLAS")
  endif()

  # AccelerateBLAS
  if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    find_package(AccelerateBLAS QUIET)
    if (NOT AccelerateBLAS_FOUND)
      list(REMOVE_ITEM AVAILABLE_BLAS_VENDORS "AccelerateBLAS")
    endif()
  endif()

  # At least one BLAS vendor must be found.
  if (NOT AVAILABLE_BLAS_VENDORS)
    message(FATAL_ERROR "No BLAS libraries found.")
  endif()

  set(${OUTPUT_VAR} ${AVAILABLE_BLAS_VENDORS} PARENT_SCOPE)
endfunction()
