SET(SRC
  contractors.cpp
)

SET(HEADERS 
  contractors.h
) 

FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

ADD_LIBRARY(contractors ${SRC})

TARGET_LINK_LIBRARIES(contractors
   ${MOOS_LIBRARIES}
   ibex
   prim
   z
   apputil
   geometry
   mbutil
   m
   pthread)