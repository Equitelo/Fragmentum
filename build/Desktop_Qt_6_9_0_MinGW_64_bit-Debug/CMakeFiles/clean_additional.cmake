# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\OpenWorld_Team3_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\OpenWorld_Team3_autogen.dir\\ParseCache.txt"
  "OpenWorld_Team3_autogen"
  )
endif()
