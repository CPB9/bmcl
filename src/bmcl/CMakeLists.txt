if(MSVC OR MINGW)
    add_definitions(-DBUILDING_BMCL)
endif()

bmcl_add_library(bmcl SHARED
    AlignedUnion.h
    Alloca.h
    ArrayView.h
    Assert.cpp
    Assert.h
    BitArray.h
    Buffer.cpp
    Buffer.h
    Bytes.cpp
    Bytes.h
    ColorStream.cpp
    ColorStream.h
    CString.cpp
    CString.h
    DoubleEq.cpp
    DoubleEq.h
    Either.h
    Endian.h
    FileUtils.cpp
    FileUtils.h
    IpAddress.cpp
    IpAddress.h
    Logging.cpp
    Logging.h
    MemReader.cpp
    MemReader.h
    MemWriter.cpp
    MemWriter.h
    MmapOpener.cpp
    MmapOpener.h
    NonNullUniquePtr.h
    Option.h
    OptionPtr.h
    Panic.cpp
    Panic.h
    PtrUtils.h
    Rc.h
    RcHash.h
    Reader.h
    RefCountable.h
    Result.h
    RingBuffer.cpp
    RingBuffer.h
    Sha3.cpp
    Sha3.h
    String.cpp
    String.h
    StringView.cpp
    StringView.h
    StringViewHash.h
    ThreadSafeRefCountable.cpp
    ThreadSafeRefCountable.h
    Utils.h
    Uuid.cpp
    Uuid.h
    UuidHash.h
    Variant.h
    Varuint.cpp
    Varuint.h
    Writer.h
    ZigZag.h
)

if(MINGW)
    target_link_libraries(bmcl shlwapi)
endif()

if (MSVC)
    target_link_libraries(bmcl Shlwapi)
endif()

if (UNIX)
    target_link_libraries(bmcl uuid)
endif()


if (BMCL_HAVE_QT)
    target_link_libraries(bmcl Qt5::Core)
endif()

target_include_directories(bmcl
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

