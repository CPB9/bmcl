if(MSVC)
    add_definitions(/Dinline=__inline) # temporary hack
else()
    add_definitions(-std=c99)
endif()

include_directories(${BMCL_INCLUDE_DIRS})

set(BMCL_SRC
    memreader.c
    memwriter.c
    ringarray.c
    ringbucket.c
    ringbuf.c
)

set(BMCL_HDR
    ${BMCL_INCLUDE_DIRS}/bmcl/endian.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memreader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/memwriter.h
    ${BMCL_INCLUDE_DIRS}/bmcl/platform.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringarray.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbucket.h
    ${BMCL_INCLUDE_DIRS}/bmcl/ringbuf.h
)

if(ARDUINO_BOARD)
    generate_arduino_library(bmcl
        SRCS ${BMCL_SRC}
        HDRS ${BMCL_HDR}
        BOARD ${ARDUINO_BOARD}
    )

    generate_arduino_firmware(ardu-test
        SRCS ardu-test.cpp
        LIBS bmcl
        BOARD ${ARDUINO_BOARD}
    )
else()
    add_library(bmcl STATIC ${BMCL_SRC})
endif()

