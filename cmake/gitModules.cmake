find_package(Git QUIET)
SET(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  joystick
  GIT_REPOSITORY https://github.com/drewnoakes/joystick.git
  GIT_TAG        master
)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/whoshuu/cpr.git
  GIT_TAG        master
  GIT_PROGRESS   TRUE
)  


FetchContent_GetProperties(joystick)
if(NOT joystick_POPULATED)
  FetchContent_Populate(joystick)
  include_directories(${joystick_SOURCE_DIR})
endif()

FetchContent_GetProperties(cpr)
if(NOT cpr_POPULATED)
  FetchContent_Populate(
    cpr
  )
  add_subdirectory(${cpr_SOURCE_DIR})
  include_directories(${CPR_INCLUDE_DIRS})
endif()