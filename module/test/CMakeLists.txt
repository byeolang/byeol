CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
include(FetchContent) # requires 3.14

# fetching googletest dependency(v1.17.0):
#   FetchContext shares parent context(e.g. cmake variables),
#   so I have to declare it at first.
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip)
#   for Windows, prevent overriding the parent porject's compiler settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# fetching flagStacker dependency:
#FetchContent_Declare(
#    flagStacker_dep
#    SOURCE_DIR /home/kniz/repo/byeolang/byeol/external/flagStacker/module)
#    #SOURCE_SUBDIR module)
#    #URL https://github.com/byeolang/flagStacker/archive/?.zip)
FetchContent_MakeAvailable(googletest)
message(STATUS "googletest source = ${googletest_SOURCE_DIR}")
message(STATUS "googletest binary = ${googletest_BINARY_DIR}")


set(moduleName "test")
set(dependencies ${dependencies} indep clog meta memlite core stela frontend Threads::Threads GTest::gtest_main)
IF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(dependencies ${dependencies} dl) # dynamic linking lib of posix compatiables.
ENDIF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)

message("building ${moduleName}..." "${moduleName}=1")

foreach(d ${dependencies})
    set(libraries ${libraries} PUBLIC ${d})
endforeach(d)

message("\tbuilding ${moduleName} dependents to ${dependencies}")

#      define sources:
file(GLOB_RECURSE sources
    "*.cc"
)
#          logging sources:
foreach(e ${sources})
    message("\tembeding ${e}")
endforeach(e)
foreach(e ${dependencies})
    message("\tlinking ${e}")
endforeach(e)

add_executable(${moduleName} ${sources})

add_custom_command(TARGET ${moduleName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${BY_TESTDATA_SRC_PATH}
        ${BY_TESTDATA_PATH}
    COMMENT "copying testdata to bin directory"
)

find_package(Threads REQUIRED)
target_link_libraries(${moduleName} PRIVATE ${libraries})

include(GoogleTest)
gtest_discover_tests(${moduleName})

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
