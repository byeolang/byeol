cmake_minimum_required (VERSION 3.14)

# include build-common if this is host project.
get_property(build_common_included GLOBAL PROPERTY BUILD_COMMON_INCLUDED)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../external/build-common)

if(NOT build_common_included)
    include(buildCommonPre)
endif()

project(byeol C CXX)

if(NOT build_common_included)
    include(buildCommon)

    # parse version
    by_parse_ver("${CMAKE_CURRENT_SOURCE_DIR}/../CHANGELOGS.md")
    set(VERSION_NAME "Mana")
endif()

# dependencies check:
by_check_dependency(NAMES git) # version is nothing matter.
by_check_dependency(NAMES bison MIN_VERSION 3.8.0)
by_check_dependency(NAMES flex MIN_VERSION 2.6.0)
by_check_dependency(NAMES make MIN_VERSION 3.0.0)

if(EMSCRIPTEN)
    # you just need to have emmake. version is nothing matter.
    by_check_dependency(NAMES emmake)
    by_check_dependency(NAMES emcmake)
endif(EMSCRIPTEN)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../module ${CMAKE_CURRENT_BINARY_DIR}/module)
