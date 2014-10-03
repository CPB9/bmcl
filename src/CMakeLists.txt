add_definitions(-std=c89)

include_directories(${BMCL_INCLUDE_DIRS})

add_library(bmcl
    ringbuf.c
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbuf.h
)

