set(nedmalloc_SRCS
    malloc.c.h
	nedmalloc.c
	nedmalloc.h
)

string(REPLACE "_DEBUG" "DEBUG" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "_DEBUG" "DEBUG" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(nedmalloc STATIC
  ${nedmalloc_SRCS}
)