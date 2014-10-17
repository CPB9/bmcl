set(BMCL_CORE_SRC
    memreader.c
    memwriter.c
    ringarray.c
    ringbucket.c
    ringbuf.c
    sched.c
)

set(BMCL_CORE_HDR
    ${BMCL_INCLUDE_DIRS}/bmcl/core/endian.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/memreader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/memwriter.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/platform.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/queue.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/reader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/ringarray.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/ringbucket.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/ringbuf.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/sched.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/writer.h
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

