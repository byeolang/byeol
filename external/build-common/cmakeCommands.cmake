message("┏━━━━━━━━━━━━━━━━━━━━━━━┓")
message("┃setting cmake commands.┃")
message("┗━━━━━━━━━━━━━━━━━━━━━━━┛") 

cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0042 NEW)

function(_by_read_dependency_version EXECUTABLE VERSION_REGEX OUT_VERSION OUT_RESULT OUT_TEXT)
    execute_process(
        COMMAND "${EXECUTABLE}" ${ARGN}
        OUTPUT_VARIABLE _by_stdout
        ERROR_VARIABLE _by_stderr
        RESULT_VARIABLE _by_rc
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )

    set(_by_version_text "${_by_stdout}")
    if(NOT "${_by_stderr}" STREQUAL "")
        if(NOT "${_by_version_text}" STREQUAL "")
            string(APPEND _by_version_text "\n")
        endif()
        string(APPEND _by_version_text "${_by_stderr}")
    endif()

    set(_by_parsed_version "")
    if(_by_rc EQUAL 0)
        string(REGEX MATCH "${VERSION_REGEX}" _by_matched "${_by_version_text}")
        if(_by_matched)
            set(_by_parsed_version "${CMAKE_MATCH_1}")
        endif()
    endif()

    set(${OUT_VERSION} "${_by_parsed_version}" PARENT_SCOPE)
    set(${OUT_RESULT} "${_by_rc}" PARENT_SCOPE)
    set(${OUT_TEXT} "${_by_version_text}" PARENT_SCOPE)
endfunction()

# by_check_dependency checks whether at least one candidate program exists and,
# when requested, whether its parsed version is greater than or equal to the
# given minimum version.
#
# Example:
# by_check_dependency(
#     NAMES python3 python
#     MIN_VERSION 3.10.0
# )
#
# Required arguments:
# - `NAMES`: candidate executable names to search for in order.
#
# Optional arguments:
# - `MIN_VERSION`: minimum accepted version. If omitted, only existence is checked.
# - `VERSION_ARGS`: arguments passed to the executable when reading its version.
#   Defaults to `--version`.
function(by_check_dependency)
    set(options)
    set(oneValueArgs MIN_VERSION)
    set(multiValueArgs NAMES VERSION_ARGS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "by_check_dependency: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    if(NOT ARG_NAMES)
        message(FATAL_ERROR "by_check_dependency: NAMES is required")
    endif()

    set(_by_display_names "")
    foreach(_by_candidate IN LISTS ARG_NAMES)
        if(IS_ABSOLUTE "${_by_candidate}")
            get_filename_component(_by_display_candidate "${_by_candidate}" NAME)
        else()
            set(_by_display_candidate "${_by_candidate}")
        endif()
        list(APPEND _by_display_names "${_by_display_candidate}")
    endforeach()
    string(JOIN " or " _by_display_name ${_by_display_names})

    if(NOT ARG_VERSION_ARGS)
        set(ARG_VERSION_ARGS --version)
    endif()

    set(_by_best_program "")
    set(_by_best_version "")
    set(_by_version_error_program "")
    set(_by_version_error_output "")
    set(_by_version_regex "([0-9]+\\.[0-9]+(\\.[0-9]+)?)")

    foreach(_by_candidate IN LISTS ARG_NAMES)
        if(IS_ABSOLUTE "${_by_candidate}" AND EXISTS "${_by_candidate}")
            set(_by_program "${_by_candidate}")
        else()
            unset(_by_program CACHE)
            unset(_by_program)
            find_program(
                _by_program
                NAMES "${_by_candidate}"
            )
        endif()

        if(NOT _by_program)
            continue()
        endif()

        if("${ARG_MIN_VERSION}" STREQUAL "")
            message(STATUS "${_by_display_name}: found (${_by_program})")
            return()
        endif()

        _by_read_dependency_version(
            "${_by_program}" "${_by_version_regex}" _by_version _by_rc _by_version_output ${ARG_VERSION_ARGS})

        if("${_by_version}" STREQUAL "")
            if("${_by_version_error_program}" STREQUAL "")
                set(_by_version_error_program "${_by_program}")
                if("${_by_version_output}" STREQUAL "")
                    set(_by_version_error_output "no version-like text was found")
                else()
                    set(_by_version_error_output "${_by_version_output}")
                endif()
            endif()
            continue()
        endif()

        if(NOT "${ARG_MIN_VERSION}" STREQUAL "" AND _by_version VERSION_LESS ARG_MIN_VERSION)
            if("${_by_best_program}" STREQUAL "" OR _by_best_version VERSION_LESS _by_version)
                set(_by_best_program "${_by_program}")
                set(_by_best_version "${_by_version}")
            endif()
            continue()
        endif()

        message(STATUS "${_by_display_name}: ${_by_version} (${_by_program})")
        return()
    endforeach()

    if(NOT "${_by_best_program}" STREQUAL "")
        message(FATAL_ERROR
            "${_by_display_name}: version ${ARG_MIN_VERSION} or newer is required, but found ${_by_best_version} (${_by_best_program}).")
    endif()

    if(NOT "${_by_version_error_program}" STREQUAL "")
        message(FATAL_ERROR
            "${_by_display_name}: found ${_by_version_error_program}, but its version could not be determined.\n${_by_version_error_output}")
    endif()

    if(NOT "${ARG_MIN_VERSION}" STREQUAL "")
        message(FATAL_ERROR
            "${_by_display_name}: version ${ARG_MIN_VERSION} or newer is required, but it is not installed.")
    endif()

    message(FATAL_ERROR "${_by_display_name}: required program was not found.")
endfunction()

# this extracts the first version only when the first version-like line in the changelog starts with `## vMAJOR.MINOR.PATCH`.
# If an earlier `v1.2.3` appears in a different form such as `### v1.2.3`, parsing fails.
# After calling it, these variables are available in the caller scope:
# - `BY_VER_MAJOR`
# - `BY_VER_MINOR`
# - `BY_VER_PATCH`
function(by_parse_ver CHANGELOG_PATH)
    get_filename_component(_by_changelog_path "${CHANGELOG_PATH}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")
    if(NOT EXISTS "${_by_changelog_path}")
        message(FATAL_ERROR "by_parse_ver: changelog file not found: ${_by_changelog_path}")
    endif()

    file(STRINGS "${_by_changelog_path}" _by_changelog_lines)
    foreach(_by_line IN LISTS _by_changelog_lines)
        if("${_by_line}" MATCHES "v[0-9]+\\.[0-9]+\\.[0-9]+")
            if("${_by_line}" MATCHES "^##[ ]+v([0-9]+)\\.([0-9]+)\\.([0-9]+)([ ]|$)")
                set(BY_VER_MAJOR "${CMAKE_MATCH_1}" PARENT_SCOPE)
                set(BY_VER_MINOR "${CMAKE_MATCH_2}" PARENT_SCOPE)
                set(BY_VER_PATCH "${CMAKE_MATCH_3}" PARENT_SCOPE)
                return()
            endif()

            message(FATAL_ERROR "by_parse_ver: the first version-like line must start with '## vMAJOR.MINOR.PATCH': ${_by_line}")
        endif()
    endforeach()

    message(FATAL_ERROR "by_parse_ver: no version header matching '## vMAJOR.MINOR.PATCH' was found in ${_by_changelog_path}")
endfunction()

if(NOT CMAKE_SCRIPT_MODE_FILE)
    include_directories(${BY_DEFAULT_INC_DIR})
    add_definitions(${CXX_DEFINITIONS})
endif()
message("\n")
