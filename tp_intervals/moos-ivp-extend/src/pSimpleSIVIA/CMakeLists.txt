#--------------------------------------------------------
# The CMakeLists.txt for:                       pSimpleSIVIA
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  SimpleSIVIA.cpp
  SimpleSIVIA_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSimpleSIVIA ${SRC})

TARGET_LINK_LIBRARIES(pSimpleSIVIA
   ${MOOS_LIBRARIES}
   apputil
   geometry
   mbutil
   m
   pthread
   contractors
   vibes)