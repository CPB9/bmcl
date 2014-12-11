set(BMCL_INTERP_SRCS
    Interpreter.cpp
)

set(BMCL_INTERP_HDRS
    Interpreter.h
)

add_bmcl_library(bmcl-interpreter SRCS ${BMCL_INTERP_SRCS} HDRS ${BMCL_INTERP_HDRS})
target_link_libraries(bmcl-interpreter bmcl-core)
