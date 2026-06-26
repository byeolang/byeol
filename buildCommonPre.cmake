# include guard:
get_property(build_common_included GLOBAL PROPERTY BUILD_COMMON_INCLUDED)
if(build_common_included)
    message("info: buildCommonPre.cmake should not be included more than once.")
    return()
endif()

# decide compiler:
#   variables such as CXX_COMPILER_ID and COMPILER_IS_GNUCC remain unchanged.
#   to resolve this issue, you must determine which compiler to use before calling
#   the project() function.
IF(NOT EMSCRIPTEN)
    find_program(CLANGPP_PATH clang++)
    find_program(CLANG_PATH clang)
    IF(CLANGPP_PATH)
        set(CMAKE_CXX_COMPILER "${CLANGPP_PATH}")
        set(CMAKE_C_COMPILER "${CLANG_PATH}")
    ENDIF(CLANGPP_PATH)
ENDIF(NOT EMSCRIPTEN)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Debug")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

