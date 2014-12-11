set(BMCL_CORE_SRCS
    MemReader.cpp
    MemWriter.cpp
    RingArray.cpp
    RingBucket.cpp
    RingBuffer.cpp
    Status.cpp
)

set(BMCL_CORE_HDRS
    Endian.h
    MemReader.h
    MemWriter.h
    Platform.h
    Reader.h
    RingArray.h
    RingBucket.h
    RingBuffer.h
    Stack.h
    Status.h
    Writer.h
)

add_bmcl_library(bmcl-core SRCS ${BMCL_CORE_SRCS} HDRS ${BMCL_CORE_HDRS})
