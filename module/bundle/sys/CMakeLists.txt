CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(podName "sys")

set(defCandidates "__${podName}=1")
foreach(def ${defCandidates})
    message("\tadd define ${def}")
    set(defines ${defines} -D${def})
endforeach(def)
add_definitions(${defines})

set(podOutDir "${BY_POD_OUTPUT_DIRECTORY}/${podName}")
set(dependencies ${dependencies} core)
set(BY_WRAPPER_SRC_PATH ${PROJECT_SOURCE_DIR}/bundle/sys/wrapper)
SET(BY_WRAPPER_DEST_PATH ${PROJECT_BINARY_DIR}/pod/sys/wrapper)

foreach(d ${dependencies})
    set(libraries ${libraries} PUBLIC ${d})
endforeach(d)

configure_file("manifest.stela" "${podOutDir}/manifest.stela")

#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

message("put module to ${BY_POD_OUTPUT_DIRECTORY}")

add_library(${podName} SHARED ${sources})
set_target_properties(${podName} PROPERTIES PREFIX "")
set_target_properties(${podName} PROPERTIES SUFFIX ".${BY_POD_EXTENSION}")
set_target_properties(${podName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${podOutDir}")
target_link_libraries(${podName} ${dependencies})
install(TARGETS ${podName} DESTINATION bin/pod PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
install(FILES manifest.stela DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/pod PERMISSIONS
    OWNER_WRITE OWNER_READ
    GROUP_READ
    WORLD_READ)

add_custom_command(TARGET ${podName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${BY_WRAPPER_SRC_PATH}
        ${BY_WRAPPER_DEST_PATH}
    COMMENT "copying wrapper to bin directory"
)

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${podName} PUBLIC "/Zc:preprocessor")
endif()
