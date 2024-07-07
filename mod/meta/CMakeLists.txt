CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

message("building ${moduleName}...")

set(moduleName "meta")

message("\tconfigure_files...")

#  defines:
#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)
#          logging sources:
foreach(e ${sources})
    message("\tembeding ${e}")
endforeach(e)
set(dependencies ${dependencies} clog)
foreach(e ${dependencies})
    message("\tlinking ${e}")
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
