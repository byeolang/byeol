CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(moduleName "leaf")
message("building ${moduleName}...")

message("\tconfigure_files...")
set(defCandidates "__${moduleName}=1")
foreach(def ${defCandidates})
    message("\tadd define ${def}")
    set(defines ${defines} -D${def})
endforeach(def)
add_definitions(${defines})

find_package(BISON)
find_package(FLEX)

set(LEAF_PARSER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/parser/bison)
set(LEAF_BISON_COMPILE_FLAGS "-l --feature=diagnostics-show-caret --warnings --color")
set(LEAF_FLEX_COMPILE_FLAGS "-L --header-file=${LEAF_PARSER_SRC_DIR}/leafLowscanner.hpp")

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(LEAF_FLEX_COMPILE_FLAGS "${LEAF_FLEX_COMPILE_FLAGS} --debug")
    set(LEAF_BISON_REPORT_FILENAME "bisonReport")
    set(LEAF_BISON_COMPILE_FLAGS "${LEAF_BISON_COMPILE_FLAGS} --verbose --report=all --debug --graph=${LEAF_PARSER_SRC_DIR}/${LEAF_BISON_REPORT_FILENAME}.gv")
endif (${CMAKE_BUILD_TYPE} STREQUAL Debug)

message("\tbison ${LEAF_BISON_COMPILE_FLAGS}")

bison_target(leafLowparser ${LEAF_PARSER_SRC_DIR}/leafLowparser.y ${LEAF_PARSER_SRC_DIR}/leafLowparser.cpp COMPILE_FLAGS ${LEAF_BISON_COMPILE_FLAGS})
flex_target(leafLowscanner ${LEAF_PARSER_SRC_DIR}/leafLowscanner.l ${LEAF_PARSER_SRC_DIR}/leafLowscanner.cpp COMPILE_FLAGS ${LEAF_FLEX_COMPILE_FLAGS})
add_flex_bison_dependency(leafLowscanner leafLowparser)

#  defines:
#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

foreach(file ${BISON_leafLowparser_OUTPUTS})
    list(APPEND sources ${file})
endforeach(file)

foreach(file ${FLEX_leafLowscanner_OUTPUTS})
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
