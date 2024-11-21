CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(packName "sys")

set(defCandidates "__${packName}=1")
foreach(def ${defCandidates})
    message("\tadd define ${def}")
    set(defines ${defines} -D${def})
endforeach(def)
add_definitions(${defines})

set(packOutDir "${NM_PACK_OUTPUT_DIRECTORY}/${packName}")
set(dependencies ${dependencies} core)

foreach(d ${dependencies})
    set(libraries ${libraries} PUBLIC ${d})
endforeach(d)

configure_file("manifest.leaf" "${packOutDir}/manifest.leaf")

#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

message("put module to ${NM_PACK_OUTPUT_DIRECTORY}")

add_library(${packName} SHARED ${sources})
set_target_properties(${packName} PROPERTIES PREFIX "")
set_target_properties(${packName} PROPERTIES SUFFIX ".${NM_PACK_EXTENSION}")
set_target_properties(${packName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${packOutDir}")
target_link_libraries(${packName} ${dependencies})
install(TARGETS ${packName} DESTINATION bin/pack PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
install(FILES manifest.leaf DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/pack PERMISSIONS
    OWNER_WRITE OWNER_READ
    GROUP_READ
    WORLD_READ)

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${packName} PUBLIC "/Zc:preprocessor")
endif()
