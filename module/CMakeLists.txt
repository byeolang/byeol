CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

message("building ${CMAKE_CURRENT_SOURCE_DIR}...")

#   In fact, this lists shows like REVERSED stack diagram.
#   it depicts the exactly dependencies for which program should be come first.
#   The faster, The lower on Stack diagram.
add_subdirectory(indep)
add_subdirectory(clog)
add_subdirectory(meta)
add_subdirectory(memlite)
add_subdirectory(stela)
add_subdirectory(core)
IF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    IF(NOT EMSCRIPTEN)
        add_subdirectory(test)
    ENDIF(NOT EMSCRIPTEN)
ENDIF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
add_subdirectory(frontend)

IF(NOT EMSCRIPTEN)
    add_subdirectory(bundle)
ENDIF(NOT EMSCRIPTEN)
