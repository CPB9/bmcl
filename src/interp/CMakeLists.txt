set(BMCL_INTERP_SRCS
    interp.c
)

set(BMCL_INTERP_HDRS
    ${BMCL_INCLUDE_DIRS}/bmcl/interp/interp.h
)

add_bmcl_library(bmcl-interp SRCS ${BMCL_INTERP_SRCS} HDRS ${BMCL_INTERP_HDRS})
target_link_libraries(bmcl-interp bmcl-core)
