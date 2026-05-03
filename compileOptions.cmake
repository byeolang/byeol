message("┏━━━━━━━━━━━━━━━━━━━━━━━━━┓")
message("┃preparing compiler flags.┃")
message("┗━━━━━━━━━━━━━━━━━━━━━━━━━┛") 

include(CheckIncludeFile)
check_include_file(execinfo.h HAS_EXECINFO)
if(HAS_EXECINFO)
    set(CMAKE_HAS_EXECINFO 1)
else()
    set(CMAKE_HAS_EXECINFO 0)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(CMAKE_SYSTEM_NAME "Linux")
endif()

IF(CMAKE_SYSTEM_NAME STREQUAL Darwin OR CMAKE_SYSTEM_NAME STREQUAL Linux OR CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_MACOSX_RPATH 1)

    # common flags:
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-delete-null-pointer-checks -Wno-sign-compare -Wno-unused-parameter -Wno-address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs -Wno-unused-function")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-zero-variadic-macro-arguments -Wno-undefined-bool-conversion -Wno-c++20-extensions")

    IF(NOT EMSCRIPTEN)
        IF(CLANGPP_PATH)
            set(CMAKE_CXX_COMPILER "${CLANGPP_PATH}")
            set(CMAKE_C_COMPILER "${CLANG_PATH}")

            # clang flags:
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-zero-variadic-macro-arguments -Wno-undefined-bool-conversion -Wno-c++20-extensions")

            IF(COVERAGE_TOOL)
                add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
                add_link_options(-fprofile-instr-generate -fcoverage-mapping)
            ENDIF(COVERAGE_TOOL)

        ELSE(CLANGPP_PATH)
            # gcc flags:
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dangling-reference") # False positive
            #   older gccs arbitrarily optimize away member variable initialization syntax in destructors that are supposed to handle
            #   allocation new/delete correctly. It's a sort of false positive, because it works fine in newer gccs.
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-lifetime-dse")

            IF(COVERAGE_TOOL)
                add_compile_options(-fprofile-arcs -ftest-coverage)
                add_link_options(-fprofile-arcs -ftest-coverage)
            ENDIF(COVERAGE_TOOL)

        ENDIF(CLANGPP_PATH)
    ENDIF(NOT EMSCRIPTEN)

    IF(EMSCRIPTEN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wno-unused-const-variable -Wno-non-pod-varargs -Wno-unused-variable")
    ELSE(EMSCRIPTEN)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0 -DBY_DEBUG -D_GLIBCXX_DEBUG")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
    ENDIF(EMSCRIPTEN)

    IF(CMAKE_RELEASE_INCLUDE_DBG_INFO STREQUAL True)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    ENDIF(CMAKE_RELEASE_INCLUDE_DBG_INFO STREQUAL True)
    if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
        set(CMAKE_SHARED_LINKER_FLAGS "-rpath .")
        set(CMAKE_EXE_LINKER_FLAGS "-rpath .")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
        if(CLANGPP_PATH)
            # Ensure libc++ runtime matches the headers provided by the detected clang.
            get_filename_component(BY_CLANG_DIR "${CLANGPP_PATH}" DIRECTORY)
            get_filename_component(BY_LLVM_ROOT "${BY_CLANG_DIR}/.." ABSOLUTE)
            set(BY_LLVM_LIBCXX_DIR "${BY_LLVM_ROOT}/lib/c++")
            if(EXISTS "${BY_LLVM_LIBCXX_DIR}/libc++.dylib")
                set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${BY_LLVM_LIBCXX_DIR} -Wl,-rpath,${BY_LLVM_LIBCXX_DIR}")
                set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L${BY_LLVM_LIBCXX_DIR} -Wl,-rpath,${BY_LLVM_LIBCXX_DIR}")
            endif()
        endif()
        set(BY_LIBRARY_EXTENSION "dylib")
    ELSE(CMAKE_SYSTEM_NAME STREQUAL Darwin)
        IF(EMSCRIPTEN)
            set(BY_LIBRARY_EXTENSION "a")
        ELSE(EMSCRIPTEN)
            set(BY_LIBRARY_EXTENSION "so")
        ENDIF(EMSCRIPTEN)
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL Darwin)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL Darwin OR CMAKE_SYSTEM_NAME STREQUAL Linux OR CMAKE_SYSTEM_NAME STREQUAL Emscripten)
IF(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /permissive- /Zc:__cplusplus /Zc:preprocessor /utf-8 /Zc:strictStrings /Zc:rvalueCast /Zc:auto /Zc:throwingNew")
    set(defines -DWIN32)
    set(BY_LIBRARY_EXTENSION "dll")
ENDIF(CMAKE_SYSTEM_NAME STREQUAL Windows)
message("\n")
