add_library(bmcl STATIC
    Alloca.h
    Assert.cpp
    Assert.h
    Buffer.cpp
    Buffer.h
    ColorStream.cpp
    ColorStream.h
    Either.h
    Endian.h
    Logging.cpp
    Logging.h
    MemReader.cpp
    MemReader.h
    MemWriter.cpp
    MemWriter.h
    Option.h
    Panic.cpp
    Panic.h
    Reader.h
    Result.h
    RingArray.cpp
    RingArray.h
    RingBucket.cpp
    RingBucket.h
    RingBuffer.cpp
    RingBuffer.h
    Utils.cpp
    Utils.h
    Writer.h
)

if (BMCL_HAVE_QT)
    target_link_libraries(bmcl Qt5::Core)
endif()
