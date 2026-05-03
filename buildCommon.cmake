include_guard(GLOBAL)

# include guard:
get_property(build_common_included GLOBAL PROPERTY BUILD_COMMON_INCLUDED)
if(build_common_included)
    message("info: build-common.cmake should not be included more than once.")
    return()
endif()
set_property(GLOBAL PROPERTY BUILD_COMMON_INCLUDED TRUE)

# decide compiler:
#   variables such as CXX_COMPILER_ID and COMPILER_IS_GNUCC remain unchanged.
#   to resolve this issue, you must determine which compiler to use before calling
#   the project() function.
IF(EMSCRIPTEN)
    set(CMAKE_C_COMPILER "emcc")
    set(CMAKE_CXX_COMPILER "emcc")
ELSE()
    find_program(CLANGPP_PATH clang++)
    find_program(CLANG_PATH clang)
    IF(CLANGPP_PATH)
        set(CMAKE_CXX_COMPILER "${CLANGPP_PATH}")
        set(CMAKE_C_COMPILER "${CLANG_PATH}")
    ENDIF(CLANGPP_PATH)
ENDIF(EMSCRIPTEN)

set(SHOW_CMAKE_VARIABLES 1)

include("${CMAKE_CURRENT_LIST_DIR}/variables.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/compileOptions.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmakeCommands.cmake")

message("┏━━━━━━━━━━━━━━━━━━━━━━━━━┓")
message("┃😎 build common prepared.┃")
message("┗━━━━━━━━━━━━━━━━━━━━━━━━━┛") 
message("all pre-declarations and paths are set.")
message("now begin to generate some makefiles for the project:")
message("\n")
