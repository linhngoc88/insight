# insight_find_blas(OUTPUT_VAR)
#
# Sets ${OUTPUT_VAR} in the caller's scope to a list of available BLAS
# options in the current system.
# Possible BLAS options are: "OpenBLAS;Atlas;Accelerate;MKL"
function(insight_find_blas OUTPUT_VAR)
  set(INSIGHT_BLAS_OPTIONS "OpenBLAS;Atlas;Accelerate;MKL")

  # Apple accelerate/vecLib
  find_package(vecLib QUIET)
  if (NOT VECLIB_FOUND)
    list(REMOVE_ITEM INSIGHT_BLAS_OPTIONS "Accelerate")
    message(STATUS "Accelerate not found!")
  endif()

  set(${OUTPUT_VAR} ${INSIGHT_BLAS_OPTIONS}
    PARENT_SCOPE)
endfunction()
