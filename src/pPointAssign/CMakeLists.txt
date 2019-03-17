#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign
# Author(s):                              Austin Jolley
#--------------------------------------------------------

SET(SRC
  PointAssign.cpp
  PointAssign_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPointAssign ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   geometry
   m
   pthread)

