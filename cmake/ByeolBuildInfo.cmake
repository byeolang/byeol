include_guard(GLOBAL)

function(byeol_extract_version_from_changelog changelog_path)
    if(NOT EXISTS "${changelog_path}")
        message(FATAL_ERROR "CHANGELOGS.md not found at ${changelog_path}")
    endif()

    file(STRINGS "${changelog_path}" _version_lines
        REGEX "^## v[0-9]+\\.[0-9]+\\.[0-9]+")
    list(LENGTH _version_lines _version_line_count)
    if(_version_line_count EQUAL 0)
        message(FATAL_ERROR
            "failed to parse the latest version from ${changelog_path}")
    endif()

    list(GET _version_lines 0 _latest_version_line)
    string(REGEX MATCH
        "^## v([0-9]+)\\.([0-9]+)\\.([0-9]+)[ \t]+(.+)[ \t]+Update$"
        _with_update "${_latest_version_line}")
    if(_with_update)
        set(_major "${CMAKE_MATCH_1}")
        set(_minor "${CMAKE_MATCH_2}")
        set(_fix "${CMAKE_MATCH_3}")
        set(_name "${CMAKE_MATCH_4}")
    else()
        string(REGEX MATCH
            "^## v([0-9]+)\\.([0-9]+)\\.([0-9]+)[ \t]+(.+)$"
            _without_update "${_latest_version_line}")
        if(NOT _without_update)
            message(FATAL_ERROR
                "failed to parse version header: ${_latest_version_line}")
        endif()

        set(_major "${CMAKE_MATCH_1}")
        set(_minor "${CMAKE_MATCH_2}")
        set(_fix "${CMAKE_MATCH_3}")
        set(_name "${CMAKE_MATCH_4}")
    endif()

    string(STRIP "${_name}" _name)

    set(VERSION_MAJOR "${_major}" PARENT_SCOPE)
    set(VERSION_MINOR "${_minor}" PARENT_SCOPE)
    set(VERSION_FIX "${_fix}" PARENT_SCOPE)
    set(VERSION_NAME "${_name}" PARENT_SCOPE)
    set(VERSION_VALUE "${_major}.${_minor}.${_fix}" PARENT_SCOPE)
    set(VERSION "${_name} Update ${_major}.${_minor}.${_fix}" PARENT_SCOPE)
endfunction()

function(byeol_get_git_commit_hash repo_root out_var)
    if(DEFINED GIT_EXECUTABLE AND NOT GIT_EXECUTABLE STREQUAL "")
        set(_git "${GIT_EXECUTABLE}")
    else()
        find_package(Git QUIET)
        if(Git_FOUND)
            set(_git "${GIT_EXECUTABLE}")
        else()
            set(${out_var} "unknown" PARENT_SCOPE)
            return()
        endif()
    endif()

    execute_process(
        COMMAND "${_git}" rev-parse --short HEAD
        WORKING_DIRECTORY "${repo_root}"
        OUTPUT_VARIABLE _git_hash
        RESULT_VARIABLE _git_rc
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(NOT _git_rc EQUAL 0 OR _git_hash STREQUAL "")
        set(_git_hash "unknown")
    endif()

    set(${out_var} "${_git_hash}" PARENT_SCOPE)
endfunction()
