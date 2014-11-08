#pragma once

#include "bmcl/core/memwriter.h"
#include "bmcl/core/memreader.h"
#include "bmcl/core/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BMCL_INSTR_PUSH8 = 0,
    BMCL_INSTR_PUSH16 = 1,
    BMCL_INSTR_PUSH32 = 2,
    BMCL_INSTR_PUSH64 = 3,
} bmcl_sci_instr_type_t;

typedef struct {
    bmcl_memreader_t bytecode;
    bmcl_memwriter_t stack;
} bmcl_sci_interp_t;

void bmcl_sci_interp_init(bmcl_sci_interp_t* self, const void* bytecode, size_t bytecode_size, void* stack,
                          size_t stack_size);

#if BMCL_HAVE_MALLOC

bmcl_sci_interp_t* bmcl_sci_interp_create(const void* bytecode, size_t bytecode_size, size_t stack_size);

void bmcl_sci_interp_destroy(bmcl_sci_interp_t* self);

#endif

bmcl_status_t bmcl_sci_interp_exec_next(bmcl_sci_interp_t* self);

#ifdef __cplusplus
}
#endif
