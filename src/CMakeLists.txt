add_definitions(-std=c99)

include_directories(${BMCL_INCLUDE_DIRS})

add_library(bmcl
    ringbuf.c
    memwriter.c
    ${BMCL_INCLUDE_DIRS}/bmcl/platform.h
    ${BMCL_INCLUDE_DIRS}/bmcl/endian.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbuf.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memwriter.h
)

