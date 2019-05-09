#--------------------------------------------------------
# The CMakeLists.txt for:                       pTempPassing
# Author(s):                              Austin Jolley
#--------------------------------------------------------

SET(SRC
  TempPassing.cpp
  TempPassing_Info.cpp
  main.cpp
  TempEntry.cpp
)

ADD_EXECUTABLE(pTempPassing ${SRC})

TARGET_LINK_LIBRARIES(pTempPassing
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   ufield
   pthread)

