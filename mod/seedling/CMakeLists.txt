CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

message("building ${moduleName}...")
set(moduleName "seedling")

message("\tconfigure_files...")

find_package(BISON)
find_package(FLEX)

bison_target(seedlingParser parser/seedlingParser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser/seedlingParser.cpp COMPILE_FLAGS "-l")
flex_target(seedlingScanner parser/seedlingScanner.l ${CMAKE_CURRENT_SOURCE_DIR}/parser/seedlingScanner.cpp COMPILE_FLAGS "-L")
add_flex_bison_dependency(seedlingScanner seedlingParser)

#  defines:
#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

foreach(file ${BISON_seedlingParser_OUTPUTS})
    list(APPEND sources ${file})
endforeach(file)

foreach(file ${FLEX_seedlingScanner_OUTPUTS})
    list(APPEND sources ${file})
endforeach(file)

#          logging sources:
foreach(e ${sources})
    message("\tembeding ${e}")
endforeach(e)
set(dependencies ${dependencies} memlite)
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
