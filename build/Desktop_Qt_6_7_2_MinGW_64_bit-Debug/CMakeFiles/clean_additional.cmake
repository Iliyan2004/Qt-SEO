# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SEO_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SEO_autogen.dir\\ParseCache.txt"
  "SEO_autogen"
  )
endif()
