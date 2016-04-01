#--------------------------------------------------------
# The CMakeLists.txt for:                       uJoystick
# Author(s):                              Vincent Drevelle, Simon Rohou
#--------------------------------------------------------

SET(SRC
  Joystick.cpp
  Joystick_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uJoystick ${SRC})

TARGET_LINK_LIBRARIES(uJoystick
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

