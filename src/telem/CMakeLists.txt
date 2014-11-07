set(BMCL_TELEM_SRCS
    db.c
)

set(BMCL_TELEM_HDRS
    ${BMCL_INCLUDE_DIRS}/bmcl/telem/db.h
)

add_bmcl_library(bmcl-telem SRCS ${BMCL_TELEM_SRCS} HDRS ${BMCL_TELEM_HDRS})
target_link_libraries(bmcl-telem bmcl-core)
