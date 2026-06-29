include_guard(GLOBAL)

# include guard:
get_property(build_common_included GLOBAL PROPERTY BUILD_COMMON_INCLUDED)
if(build_common_included)
    message("info: buildCommon.cmake should not be included more than once.")
    return()
endif()
set_property(GLOBAL PROPERTY BUILD_COMMON_INCLUDED TRUE)

set(SHOW_CMAKE_VARIABLES 1)

include("${CMAKE_CURRENT_LIST_DIR}/variables.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/compileOptions.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmakeCommands.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/postScript.cmake")

message("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓")
message("┃ 🚀 The project is good to go, ignition!               ┃")
message("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫") 
message("┃ all pre-declarations and paths are set.               ┃")
message("┃ now begin to generate some makefiles for the project. ┃")
message("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛")
message("\n")
message("\n")
