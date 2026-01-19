CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

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

add_executable(${moduleName} ${sources})
add_library("frontend" ${sources}) # for test module
target_link_libraries(${moduleName} ${libraries})

IF(EMSCRIPTEN)
    set_target_properties(byeol PROPERTIES SUFFIX ".js" LINK_FLAGS "-s WASM=1 -std=c++17")
ELSE(EMSCRIPTEN)
    install(TARGETS ${moduleName} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE)
ENDIF(EMSCRIPTEN)

#       let MSVC follow macro expansion of standard:
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(${moduleName} PUBLIC "/Zc:preprocessor")
endif()
