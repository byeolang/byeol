CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(packName "sys")

set(defCandidates "__${packName}=1")
foreach(def ${defCandidates})
    message("\tadd define ${def}")
    set(defines ${defines} -D${def})
endforeach(def)
add_definitions(${defines})

set(packOutDir "${BY_PACK_OUTPUT_DIRECTORY}/${packName}")
set(dependencies ${dependencies} core)
set(BY_WRAPPER_SRC_PATH ${PROJECT_SOURCE_DIR}/bundle/sys/wrapper)
SET(BY_WRAPPER_DEST_PATH ${PROJECT_BINARY_DIR}/pack/sys/wrapper)

foreach(d ${dependencies})
    set(libraries ${libraries} PUBLIC ${d})
endforeach(d)

configure_file("manifest.stela" "${packOutDir}/manifest.stela")

#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

message("put module to ${BY_PACK_OUTPUT_DIRECTORY}")

add_library(${packName} SHARED ${sources})
set_target_properties(${packName} PROPERTIES PREFIX "")
set_target_properties(${packName} PROPERTIES SUFFIX ".${BY_PACK_EXTENSION}")
set_target_properties(${packName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${packOutDir}")
target_link_libraries(${packName} ${dependencies})
install(TARGETS ${packName} DESTINATION bin/pack PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
install(FILES manifest.stela DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/pack PERMISSIONS
    OWNER_WRITE OWNER_READ
    GROUP_READ
    WORLD_READ)

add_custom_command(TARGET ${packName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${BY_WRAPPER_SRC_PATH}
        ${BY_WRAPPER_DEST_PATH}
    COMMENT "copying wrapper to bin directory"
)

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${packName} PUBLIC "/Zc:preprocessor")
endif()
