#--------------------------------------------------------
# The CMakeLists.txt for:                       uSimModem
# Author(s):                              ENSTA Bretagne
#--------------------------------------------------------

SET(SRC
  SimModem.cpp
  SimModem_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uSimModem ${SRC})

TARGET_LINK_LIBRARIES(uSimModem
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

