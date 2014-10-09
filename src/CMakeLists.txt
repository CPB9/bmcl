add_definitions(-std=c99)

include_directories(${BMCL_INCLUDE_DIRS})

add_library(bmcl
    memreader.c
    memwriter.c
    ringarray.c
    ringbucket.c
    ringbuf.c
    ${BMCL_INCLUDE_DIRS}/bmcl/endian.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memreader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memwriter.h
    ${BMCL_INCLUDE_DIRS}/bmcl/platform.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringarray.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbucket.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbuf.h
)

