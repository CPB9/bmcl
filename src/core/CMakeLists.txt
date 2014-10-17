set(BMCL_CORE_SRC
    memreader.c
    memwriter.c
    ringarray.c
    ringbucket.c
    ringbuf.c
    sched.c
)

set(BMCL_CORE_HDR
    ${BMCL_INCLUDE_DIRS}/bmcl/endian.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memreader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memwriter.h
    ${BMCL_INCLUDE_DIRS}/bmcl/platform.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringarray.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbucket.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbuf.h
    ${BMCL_INCLUDE_DIRS}/bmcl/sched.h
)

if(ARDUINO_BOARD)
    generate_arduino_library(bmclcore
        SRCS ${BMCL_CORE_SRC}
        HDRS ${BMCL_CORE_HDR}
        BOARD ${ARDUINO_BOARD}
    )
else()
    add_library(bmclcore
        STATIC
        ${BMCL_CORE_SRC}
        ${BMCL_CORE_HDR}
    )
endif()

