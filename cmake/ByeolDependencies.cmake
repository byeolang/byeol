include_guard(GLOBAL)

function(_byeol_read_program_version executable version_regex out_var)
    execute_process(
        COMMAND "${executable}" ${ARGN}
        OUTPUT_VARIABLE _stdout
        ERROR_VARIABLE _stderr
        RESULT_VARIABLE _rc
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _rc EQUAL 0)
        message(FATAL_ERROR
            "failed to query ${executable} version:\n${_stdout}\n${_stderr}")
    endif()

    set(_version_text "${_stdout}\n${_stderr}")
    string(REGEX MATCH "${version_regex}" _matched "${_version_text}")
    if(NOT _matched)
        message(FATAL_ERROR
            "failed to parse ${executable} version from:\n${_version_text}")
    endif()

    set(${out_var} "${CMAKE_MATCH_1}" PARENT_SCOPE)
endfunction()

function(_byeol_require_program)
    set(options)
    set(oneValueArgs NAME MIN_VERSION VERSION_REGEX OUT_EXECUTABLE OUT_VERSION)
    set(multiValueArgs NAMES VERSION_ARGS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_best_program "")
    set(_best_version "")

    foreach(_candidate IN LISTS ARG_NAMES)
        find_program(_program NAMES "${_candidate}")
        if(NOT _program)
            continue()
        endif()

        _byeol_read_program_version(
            "${_program}" "${ARG_VERSION_REGEX}" _program_version ${ARG_VERSION_ARGS})

        if(_program_version VERSION_LESS ARG_MIN_VERSION)
            if(_best_program STREQUAL "" OR _best_version VERSION_LESS _program_version)
                set(_best_program "${_program}")
                set(_best_version "${_program_version}")
            endif()
            continue()
        endif()

        message(STATUS "${ARG_NAME}: ${_program_version} (${_program})")
        if(ARG_OUT_EXECUTABLE)
            set(${ARG_OUT_EXECUTABLE} "${_program}" PARENT_SCOPE)
        endif()
        if(ARG_OUT_VERSION)
            set(${ARG_OUT_VERSION} "${_program_version}" PARENT_SCOPE)
        endif()
        return()
    endforeach()

    if(_best_program STREQUAL "")
        message(FATAL_ERROR
            "${ARG_NAME} version ${ARG_MIN_VERSION}+ is not installed")
    endif()

    message(FATAL_ERROR
        "${ARG_NAME} version should be ${ARG_MIN_VERSION}+ but yours is ${_best_version} (${_best_program})")
endfunction()

function(_byeol_check_compiler)
    if(MSVC)
        set(_compiler_name "msvc")
        set(_compiler_version "${CMAKE_CXX_COMPILER_VERSION}")
        set(_compiler_path "${CMAKE_CXX_COMPILER}")
        if(MSVC_VERSION LESS 1930)
            message(FATAL_ERROR
                "${_compiler_name} version should be 17.0+ but yours is ${_compiler_version} (${_compiler_path})")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(_compiler_name "clang++")
        set(_compiler_version "${CMAKE_CXX_COMPILER_VERSION}")
        set(_compiler_path "${CMAKE_CXX_COMPILER}")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0.0)
            message(FATAL_ERROR
                "${_compiler_name} version should be 14.0.0+ but yours is ${_compiler_version} (${_compiler_path})")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(_compiler_name "g++")
        set(_compiler_version "${CMAKE_CXX_COMPILER_VERSION}")
        set(_compiler_path "${CMAKE_CXX_COMPILER}")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0.0)
            message(FATAL_ERROR
                "${_compiler_name} version should be 8.0.0+ but yours is ${_compiler_version} (${_compiler_path})")
        endif()
    else()
        message(FATAL_ERROR
            "unsupported C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    endif()

    message(STATUS "${_compiler_name}: ${_compiler_version} (${_compiler_path})")
endfunction()

function(byeol_check_build_dependencies)
    message(STATUS "CMake: ${CMAKE_VERSION} (${CMAKE_COMMAND})")
    _byeol_check_compiler()

    _byeol_require_program(
        NAME "git"
        NAMES git
        MIN_VERSION 2.20.0
        VERSION_REGEX "([0-9]+\\.[0-9]+\\.[0-9]+)"
        VERSION_ARGS --version
        OUT_EXECUTABLE GIT_EXECUTABLE
        OUT_VERSION GIT_VERSION
    )

    find_package(BISON REQUIRED)
    if(BISON_VERSION VERSION_LESS 3.8.0)
        message(FATAL_ERROR
            "bison version should be 3.8.0+ but yours is ${BISON_VERSION} (${BISON_EXECUTABLE})")
    endif()
    message(STATUS "bison: ${BISON_VERSION} (${BISON_EXECUTABLE})")

    find_package(FLEX REQUIRED)
    if(FLEX_VERSION VERSION_LESS 2.6.0)
        message(FATAL_ERROR
            "flex version should be 2.6.0+ but yours is ${FLEX_VERSION} (${FLEX_EXECUTABLE})")
    endif()
    message(STATUS "flex: ${FLEX_VERSION} (${FLEX_EXECUTABLE})")

    if(NOT CMAKE_SYSTEM_NAME STREQUAL Windows AND CMAKE_GENERATOR MATCHES "Makefiles")
        if(CMAKE_MAKE_PROGRAM)
            set(_make_program "${CMAKE_MAKE_PROGRAM}")
            _byeol_read_program_version(
                "${_make_program}" "([0-9]+\\.[0-9]+(\\.[0-9]+)?)" _make_version --version)
            if(_make_version VERSION_LESS 3.0.0)
                message(FATAL_ERROR
                    "make version should be 3.0.0+ but yours is ${_make_version} (${_make_program})")
            endif()
            message(STATUS "make: ${_make_version} (${_make_program})")
        else()
            _byeol_require_program(
                NAME "make"
                NAMES make gmake
                MIN_VERSION 3.0.0
                VERSION_REGEX "([0-9]+\\.[0-9]+(\\.[0-9]+)?)"
                VERSION_ARGS --version
            )
        endif()
    endif()
endfunction()
