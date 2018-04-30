if(MSVC OR MINGW)
    add_definitions(-DBMCL_DLL)
endif()

bmcl_add_library(bmcl SHARED
    Alloca.h
    ArrayView.h
    AlignedUnion.h
    Assert.cpp
    Assert.h
    BitArray.h
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
    NonNullUniquePtr.h
    IpAddress.h
    IpAddress.cpp
    Option.h
    OptionFwd.h
    OptionPtr.h
    Panic.cpp
    Panic.h
    PtrUtils.h
    Rc.h
    RefCountable.h
    Reader.h
    Result.h
    ResultFwd.h
    RingArray.cpp
    RingArray.h
    RingBucket.cpp
    RingBucket.h
    RingBuffer.cpp
    RingBuffer.h
    Sha3.cpp
    Sha3.h
    String.h
    String.cpp
    StringView.cpp
    StringView.h
    ThreadSafeRefCountable.h
    ThreadSafeRefCountable.cpp
    Variant.h
    FileUtils.cpp
    FileUtils.h
    Utils.h
    Writer.h
)

if(MINGW)
    target_link_libraries(bmcl shlwapi)
endif()

if (MSVC)
    target_link_libraries(bmcl Shlwapi)
endif()

if (BMCL_HAVE_QT)
    target_link_libraries(bmcl Qt5::Core)
endif()

target_include_directories(bmcl
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

