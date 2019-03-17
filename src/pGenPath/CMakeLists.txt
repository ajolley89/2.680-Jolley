#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenPath
# Author(s):                              Austin Jolley
#--------------------------------------------------------

SET(SRC
  GenPath.cpp
  GenPath_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pGenPath ${SRC})

TARGET_LINK_LIBRARIES(pGenPath
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)
   geometry

