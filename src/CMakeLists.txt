if(MSVC)
    add_definitions(/Dinline=__inline) # temporary hack
else()
    add_definitions(-std=c99)
endif()

include_directories(${BMCL_INCLUDE_DIRS})

add_subdirectory(core)

