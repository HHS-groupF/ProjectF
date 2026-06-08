# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RPI-WEMOS_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RPI-WEMOS_autogen.dir/ParseCache.txt"
  "RPI-WEMOS_autogen"
  )
endif()
