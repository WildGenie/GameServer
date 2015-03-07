set(nedmalloc_SRCS
    malloc.c.h
	nedmalloc.c
	nedmalloc.h
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(nedmalloc STATIC
  ${nedmalloc_SRCS}
)
