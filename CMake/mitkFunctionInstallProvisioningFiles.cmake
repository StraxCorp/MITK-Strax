#[=[function(mitkFunctionInstallProvisioningFiles)
  foreach(file ${ARGN})
    get_filename_component(file_name "${file}" NAME)
    MITK_INSTALL(FILES ${file}.install RENAME "${file_name}")
  endforeach()
endfunction()]=]
