set(BMCL_CORE_SRCS
    ringarray.cpp
    ringbucket.cpp
    ringbuf.cpp
    status.cpp
)

set(BMCL_CORE_HDRS
    ${BMCL_INCLUDE_DIRS}/bmcl/core/endian.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/memreader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/memwriter.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/platform.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/reader.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/ringarray.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/ringbucket.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/ringbuf.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/status.h
    ${BMCL_INCLUDE_DIRS}/bmcl/core/writer.h
)

add_bmcl_library(bmcl-core SRCS ${BMCL_CORE_SRCS} HDRS ${BMCL_CORE_HDRS})
