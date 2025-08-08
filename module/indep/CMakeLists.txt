CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(moduleName "indep")
message("building ${moduleName}...")

message("\tconfigure_files...")
set(defCandidates "__${moduleName}=1")
foreach(def ${defCandidates})
    message("\tadd define ${def}")
    set(defines ${defines} -D${def})
endforeach(def)
add_definitions(${defines})

#  expanding:
set(configures
    "def/buildInformation.hpp.in"
)
foreach(e ${configures})
    #  extract output filepath:
    string(FIND ${e} ".in" n REVERSE)
    string(SUBSTRING ${e} 0 ${n} output_path)
    message("\texpanding ${CMAKE_CURRENT_SOURCE_DIR}/${output_path}")
    configure_file(${e} ${CMAKE_CURRENT_SOURCE_DIR}/${output_path})
endforeach(e)
#  defines:
#      define libraries:
foreach(e ${dependencies})
    message("\tlinking ${e}")
endforeach(e)
#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)
#          logging sources:
foreach(e ${sources})
    message("\tembeding ${e}")
endforeach(e)

#  execute:
IF(EMSCRIPTEN)
    add_library(${moduleName} ${sources})
ELSE(EMSCRIPTEN)
    add_library(${moduleName} SHARED ${sources})
    install(TARGETS ${moduleName} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE)
ENDIF(EMSCRIPTEN)
set_target_properties(${moduleName} PROPERTIES SUFFIX ".${BY_LIBRARY_EXTENSION}")
target_link_libraries(${moduleName} ${dependencies})

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
