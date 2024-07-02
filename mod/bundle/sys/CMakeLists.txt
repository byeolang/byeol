CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(packName "sys")
set(packOutDir "${NM_PACK_OUTPUT_DIRECTORY}/${packName}")
set(dependencies ${dependencies} core)

set(defCandidates "NM_TAG=\"${packName}\"" "__${packName}=1")
foreach(def ${defCandidates})
    message("\tadd define ${def}")
    set(defines ${defines} -D${def})
endforeach(def)
add_definitions(${defines})

foreach(d ${dependencies})
    set(libraries ${libraries} PUBLIC ${d})
endforeach(d)

configure_file("manifest.seedling" "${packOutDir}/manifest.seedling")

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

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${packName} PUBLIC "/Zc:preprocessor")
endif()
