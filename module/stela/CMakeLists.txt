CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(moduleName "stela")
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

set(STELA_PARSER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/parser/bison)
set(STELA_BISON_COMPILE_FLAGS "-l --feature=diagnostics-show-caret --warnings --color")
set(STELA_FLEX_COMPILE_FLAGS "-L --header-file=${STELA_PARSER_SRC_DIR}/stelaLowscanner.hpp")

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(STELA_FLEX_COMPILE_FLAGS "${STELA_FLEX_COMPILE_FLAGS} --debug")
    set(STELA_BISON_REPORT_FILENAME "bisonReport")
    set(STELA_BISON_COMPILE_FLAGS "${STELA_BISON_COMPILE_FLAGS} --verbose --report=all --debug --graph=${STELA_PARSER_SRC_DIR}/${STELA_BISON_REPORT_FILENAME}.gv")
endif (${CMAKE_BUILD_TYPE} STREQUAL Debug)

message("\tbison ${STELA_BISON_COMPILE_FLAGS}")

bison_target(stelaLowparser ${STELA_PARSER_SRC_DIR}/stelaLowparser.y ${STELA_PARSER_SRC_DIR}/stelaLowparser.cpp COMPILE_FLAGS ${STELA_BISON_COMPILE_FLAGS})
flex_target(stelaLowscanner ${STELA_PARSER_SRC_DIR}/stelaLowscanner.l ${STELA_PARSER_SRC_DIR}/stelaLowscanner.cpp COMPILE_FLAGS ${STELA_FLEX_COMPILE_FLAGS})
add_flex_bison_dependency(stelaLowscanner stelaLowparser)

#  defines:
#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

foreach(file ${BISON_stelaLowparser_OUTPUTS})
    list(APPEND sources ${file})
endforeach(file)

foreach(file ${FLEX_stelaLowscanner_OUTPUTS})
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
