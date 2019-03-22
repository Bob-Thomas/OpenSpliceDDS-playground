FetchContent_Declare(
  joystick
  GIT_REPOSITORY https://github.com/drewnoakes/joystick.git
  GIT_TAG        master
)

FetchContent_GetProperties(joystick)
if(NOT joystick_POPULATED)
  FetchContent_Populate(joystick)
  include_directories(${joystick_SOURCE_DIR})
endif()