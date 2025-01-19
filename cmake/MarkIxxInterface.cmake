function(mark_ixx_as_module_interface ROOT_DIR)
    if(MSVC)
        file(GLOB_RECURSE IXX_FILES
            CONFIGURE_DEPENDS
            "${ROOT_DIR}/*.ixx"
        )

        foreach(ixx_file IN LISTS IXX_FILES)
            message(STATUS "Marking as module interface: ${ixx_file}")
            set_source_files_properties("${ixx_file}" PROPERTIES VS_MODULE_INTERFACE TRUE)
        endforeach()
    else()
        message(STATUS "Not MSVC, so skipping VS_MODULE_INTERFACE.")
    endif()
endfunction()
