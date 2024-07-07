CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

message("building ${moduleName}...")
set(moduleName "indep")

message("\tconfigure_files...")
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
ENDIF(EMSCRIPTEN)
set_target_properties(${moduleName} PROPERTIES SUFFIX ".${NM_LIBRARY_EXTENSION}")
target_link_libraries(${moduleName} ${dependencies})

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
