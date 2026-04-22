CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

include(FetchContent) # requires 3.14

# fetching flagStacker dependency:
#   FetchContext shares parent context(e.g. cmake variables),
#   so I have to declare it at first.
FetchContent_Declare(
    flagStacker_dep
    SOURCE_DIR /home/kniz/repo/byeolang/byeol/external/flagStacker/module)
    #SOURCE_SUBDIR module)
    #URL https://github.com/byeolang/flagStacker/archive/?.zip)
FetchContent_MakeAvailable(googletest flagStacker_dep)
message(STATUS "flagStacker source = ${flagstacker_dep_SOURCE_DIR}")
message(STATUS "flagStacker binary = ${flagstacker_dep_BINARY_DIR}")

set(moduleName "byeol")
set(dependencies ${dependencies} core)
IF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(dependencies ${dependencies} dl) # dynamic linking lib of posix compatiables.
ENDIF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)

message("building ${moduleName}...")

foreach(d ${dependencies})
    set(libraries ${libraries} PUBLIC ${d})
endforeach(d)

message("\tbuilding ${moduleName} dependents to ${dependencies}")

#      define sources:
file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)
#          logging sources:
foreach(e ${sources})
    message("\tembeding ${e}")
endforeach(e)
foreach(e ${dependencies})
    message("\tlinking ${e}")
endforeach(e)


add_library("frontend" ${sources}) # for test module
target_link_libraries("frontend" ${libraries} flagStacker)
add_executable(${moduleName} ${sources})
target_link_libraries(${moduleName}
    PRIVATE ${libraries} flagStacker)

IF(EMSCRIPTEN)
    set_target_properties(${moduleName} PROPERTIES SUFFIX ".js" LINK_FLAGS "-s WASM=1 -std=c++17")
ELSE(EMSCRIPTEN)
    install(TARGETS ${moduleName} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE)
ENDIF(EMSCRIPTEN)

get_target_property(_fs_inc flagStacker
    INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "flagStacker includes = ${_fs_inc}")

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
