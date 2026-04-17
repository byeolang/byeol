CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
include(FetchContent) # requires 3.14

set(moduleName "test")
set(dependencies ${dependencies} indep clog meta memlite core stela frontend)
IF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(dependencies ${dependencies} dl) # dynamic linking lib of posix compatiables.
ENDIF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)

message("building ${moduleName}..." "${moduleName}=1")

# use commitId of v1.17.0
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip)
#   for Windows, prevent overriding the parent porject's compiler settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
target_link_libraries(${moduleName} ${libraries}
    PRIVATE Threads::Threads GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${moduleName})

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
