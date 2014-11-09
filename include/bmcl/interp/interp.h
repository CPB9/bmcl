#pragma once

#include "bmcl/core/memwriter.h"
#include "bmcl/core/memreader.h"
#include "bmcl/core/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BMCL_INSTR_PUSH8 = 0,
    BMCL_INSTR_PUSH16,
    BMCL_INSTR_PUSH32,
    BMCL_INSTR_PUSH64,
    BMCL_INSTR_CONVERT_I8_TO_I32,
    BMCL_INSTR_CONVERT_I8_TO_U32,
    BMCL_INSTR_CONVERT_U8_TO_I32,
    BMCL_INSTR_CONVERT_U8_TO_U32,
    BMCL_INSTR_CONVERT_I16_TO_I32,
    BMCL_INSTR_CONVERT_I16_TO_U32,
    BMCL_INSTR_CONVERT_U16_TO_I32,
    BMCL_INSTR_CONVERT_U16_TO_U32,
    BMCL_INSTR_CONVERT_I32_TO_I8,
    BMCL_INSTR_CONVERT_I32_TO_U8,
    BMCL_INSTR_CONVERT_I32_TO_I16,
    BMCL_INSTR_CONVERT_I32_TO_U16,
    BMCL_INSTR_CONVERT_I32_TO_I64,
    BMCL_INSTR_CONVERT_I32_TO_U64,
    BMCL_INSTR_CONVERT_I32_TO_FLOAT,
    BMCL_INSTR_CONVERT_I32_TO_DOUBLE,
    BMCL_INSTR_CONVERT_U32_TO_I8,
    BMCL_INSTR_CONVERT_U32_TO_U8,
    BMCL_INSTR_CONVERT_U32_TO_I16,
    BMCL_INSTR_CONVERT_U32_TO_U16,
    BMCL_INSTR_CONVERT_U32_TO_U64,
    BMCL_INSTR_CONVERT_U32_TO_I64,
    BMCL_INSTR_CONVERT_U32_TO_FLOAT,
    BMCL_INSTR_CONVERT_U32_TO_DOUBLE,
    BMCL_INSTR_CONVERT_I64_TO_I32,
    BMCL_INSTR_CONVERT_I64_TO_U32,
    BMCL_INSTR_CONVERT_I64_TO_FLOAT,
    BMCL_INSTR_CONVERT_I64_TO_DOUBLE,
    BMCL_INSTR_CONVERT_U64_TO_I32,
    BMCL_INSTR_CONVERT_U64_TO_U32,
    BMCL_INSTR_CONVERT_U64_TO_FLOAT,
    BMCL_INSTR_CONVERT_U64_TO_DOUBLE,
    BMCL_INSTR_CONVERT_FLOAT_TO_I32,
    BMCL_INSTR_CONVERT_FLOAT_TO_U32,
    BMCL_INSTR_CONVERT_FLOAT_TO_I64,
    BMCL_INSTR_CONVERT_FLOAT_TO_U64,
    BMCL_INSTR_CONVERT_FLOAT_TO_DOUBLE,
    BMCL_INSTR_CONVERT_DOUBLE_TO_I32,
    BMCL_INSTR_CONVERT_DOUBLE_TO_U32,
    BMCL_INSTR_CONVERT_DOUBLE_TO_I64,
    BMCL_INSTR_CONVERT_DOUBLE_TO_U64,
    BMCL_INSTR_CONVERT_DOUBLE_TO_FLOAT,
    BMCL_INSTR_SUM_I8,
    BMCL_INSTR_SUM_I16,
    BMCL_INSTR_SUM_I32,
    BMCL_INSTR_SUM_I64,
    BMCL_INSTR_SUM_U8,
    BMCL_INSTR_SUM_U16,
    BMCL_INSTR_SUM_U32,
    BMCL_INSTR_SUM_U64,
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
