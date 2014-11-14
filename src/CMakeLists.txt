if(MSVC)
    add_definitions(/Dinline=__inline) # temporary hack
else()
    add_definitions(-std=c++11)
endif()

include_directories(${BMCL_INCLUDE_DIRS})

function(ADD_BMCL_LIBRARY lib)
    cmake_parse_arguments(ADD_BMCL_LIBRARY "" "" "SRCS;HDRS" ${ARGN})
    if(ARDUINO_BOARD)
        generate_arduino_library(${lib}
            SRCS ${ADD_BMCL_LIBRARY_SRCS}
            HDRS ${ADD_BMCL_LIBRARY_HDRS}
            BOARD ${ARDUINO_BOARD}
        )
    else()
        add_library(${lib}
            STATIC
            ${ADD_BMCL_LIBRARY_SRCS}
            ${ADD_BMCL_LIBRARY_HDRS}
        )
    endif()
endfunction()

add_subdirectory(core)
add_subdirectory(interp)
#add_subdirectory(telem)
