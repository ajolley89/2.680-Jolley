#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle_ajolley
# Author(s):                              Austin Jolley
#--------------------------------------------------------

SET(SRC
  CommunicationAngle_ajolley.cpp
  CommunicationAngle_ajolley_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pCommunicationAngle_ajolley ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle_ajolley
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

