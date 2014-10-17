set(BMCL_TM_SRCS
    db.c
)

set(BMCL_TM_HDRS
    ${BMCL_INCLUDE_DIRS}/bmcl/tm/db.h
)

add_bmcl_library(bmcltm SRCS ${BMCL_TM_SRCS} HDRS ${BMCL_TM_HDRS})
target_link_libraries(bmcltm bmclcore)
