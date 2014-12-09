set(BMCL_CORE_SRCS
    memreader.cpp
    memwriter.cpp
    ringarray.cpp
    ringbucket.cpp
    ringbuf.cpp
    status.cpp
)

set(BMCL_CORE_HDRS
    endian.h
    memreader.h
    memwriter.h
    platform.h
    reader.h
    ringarray.h
    ringbucket.h
    ringbuf.h
    stack.h
    status.h
    writer.h
)

add_bmcl_library(bmcl-core SRCS ${BMCL_CORE_SRCS} HDRS ${BMCL_CORE_HDRS})
