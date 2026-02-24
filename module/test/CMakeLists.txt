CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(moduleName "test")
set(dependencies ${dependencies} indep clog meta memlite core stela frontend ${pathGtest})
IF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(dependencies ${dependencies} dl) # dynamic linking lib of posix compatiables.
ENDIF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)

message("building ${moduleName}..." "${moduleName}=1")

find_library(pathGtest
    NAMES gtest
    REQUIRED)
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
    PRIVATE Threads::Threads)

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
