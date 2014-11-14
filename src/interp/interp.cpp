#include "bmcl/interp/interp.h"

#include <assert.h>
#include <stdlib.h>

#define MAKE_STACK_PUSH_FUNC(type, func_suffix, read_suffix, write_suffix)                                             \
    static bmcl_status_t stack_push##func_suffix(bmcl_sci_interp_t* interp)                                            \
    {                                                                                                                  \
        if (bmcl_memreader_size_left(&interp->bytecode) < sizeof(type)) {                                              \
            return BMCL_ERR_UNEXPECTED_END_OF_BYTECODE;                                                                \
        }                                                                                                              \
        if (bmcl_memwriter_size_left(&interp->stack) < sizeof(type)) {                                                 \
            return BMCL_ERR_STACK_OVERFLOW;                                                                            \
        }                                                                                                              \
        type value = bmcl_memreader_read_##read_suffix(&interp->bytecode);                                             \
        bmcl_memwriter_write_##write_suffix(&interp->stack, value);                                                    \
        return BMCL_SUCCESS;                                                                                           \
    }

MAKE_STACK_PUSH_FUNC(int8_t, 8, uint8, uint8);
MAKE_STACK_PUSH_FUNC(int16_t, 16, uint16be, uint16);
MAKE_STACK_PUSH_FUNC(int32_t, 32, uint32be, uint32);
MAKE_STACK_PUSH_FUNC(int64_t, 64, uint64be, uint64);

#define MAKE_CONVERT_FUNCTION(type1, name1, type2, name2)                                                              \
    static bmcl_status_t convert_##name1##_to_##name2(bmcl_sci_interp_t* interp)                                       \
    {                                                                                                                  \
        if (bmcl_memwriter_size(&interp->stack) < sizeof(type1)) {                                                     \
            return BMCL_ERR_NOT_ENOUGH_STACK_DATA;                                                                     \
        }                                                                                                              \
                                                                                                                       \
        type1 var1;                                                                                                    \
        bmcl_memwriter_pop(&interp->stack, &var1, sizeof(type1));                                                      \
                                                                                                                       \
        if (bmcl_memwriter_size_left(&interp->stack) < sizeof(type2)) {                                                \
            return BMCL_ERR_STACK_OVERFLOW;                                                                            \
        }                                                                                                              \
                                                                                                                       \
        type2 var2 = (type2)var1;                                                                                      \
        bmcl_memwriter_write(&interp->stack, &var2, sizeof(type2));                                                    \
        return BMCL_SUCCESS;                                                                                           \
    }

MAKE_CONVERT_FUNCTION(int8_t, i8, int32_t, i32);
MAKE_CONVERT_FUNCTION(int8_t, i8, uint32_t, u32);
MAKE_CONVERT_FUNCTION(uint8_t, u8, int32_t, i32);
MAKE_CONVERT_FUNCTION(uint8_t, u8, uint32_t, u32);
MAKE_CONVERT_FUNCTION(int16_t, i16, int32_t, i32);
MAKE_CONVERT_FUNCTION(int16_t, i16, uint32_t, u32);
MAKE_CONVERT_FUNCTION(uint16_t, u16, int32_t, i32);
MAKE_CONVERT_FUNCTION(uint16_t, u16, uint32_t, u32);
MAKE_CONVERT_FUNCTION(int32_t, i32, int8_t, i8);
MAKE_CONVERT_FUNCTION(int32_t, i32, uint8_t, u8);
MAKE_CONVERT_FUNCTION(int32_t, i32, int16_t, i16);
MAKE_CONVERT_FUNCTION(int32_t, i32, uint16_t, u16);
MAKE_CONVERT_FUNCTION(int32_t, i32, int64_t, i64);
MAKE_CONVERT_FUNCTION(int32_t, i32, uint64_t, u64);
MAKE_CONVERT_FUNCTION(int32_t, i32, float, float);
MAKE_CONVERT_FUNCTION(int32_t, i32, double, double);
MAKE_CONVERT_FUNCTION(uint32_t, u32, int8_t, i8);
MAKE_CONVERT_FUNCTION(uint32_t, u32, uint8_t, u8);
MAKE_CONVERT_FUNCTION(uint32_t, u32, int16_t, i16);
MAKE_CONVERT_FUNCTION(uint32_t, u32, uint16_t, u16);
MAKE_CONVERT_FUNCTION(uint32_t, u32, int64_t, i64);
MAKE_CONVERT_FUNCTION(uint32_t, u32, uint64_t, u64);
MAKE_CONVERT_FUNCTION(uint32_t, u32, float, float);
MAKE_CONVERT_FUNCTION(uint32_t, u32, double, double);
MAKE_CONVERT_FUNCTION(int64_t, i64, int32_t, i32);
MAKE_CONVERT_FUNCTION(int64_t, i64, uint32_t, u32);
MAKE_CONVERT_FUNCTION(int64_t, i64, float, float);
MAKE_CONVERT_FUNCTION(int64_t, i64, double, double);
MAKE_CONVERT_FUNCTION(uint64_t, u64, int32_t, i32);
MAKE_CONVERT_FUNCTION(uint64_t, u64, uint32_t, u32);
MAKE_CONVERT_FUNCTION(uint64_t, u64, float, float);
MAKE_CONVERT_FUNCTION(uint64_t, u64, double, double);
MAKE_CONVERT_FUNCTION(float, float, int32_t, i32);
MAKE_CONVERT_FUNCTION(float, float, uint32_t, u32);
MAKE_CONVERT_FUNCTION(float, float, int64_t, i64);
MAKE_CONVERT_FUNCTION(float, float, uint64_t, u64);
MAKE_CONVERT_FUNCTION(float, float, double, double);
MAKE_CONVERT_FUNCTION(double, double, int32_t, i32);
MAKE_CONVERT_FUNCTION(double, double, uint32_t, u32);
MAKE_CONVERT_FUNCTION(double, double, int64_t, i64);
MAKE_CONVERT_FUNCTION(double, double, uint64_t, u64);
MAKE_CONVERT_FUNCTION(double, double, float, float);


#define MAKE_MATH_FUNCTION(type, prefix, suffix, op)                                                                   \
    static bmcl_status_t prefix##_##suffix(bmcl_sci_interp_t* interp)                                                  \
    {                                                                                                                  \
        if (bmcl_memwriter_size(&interp->stack) < 2 * sizeof(type)) {                                                  \
            return BMCL_ERR_NOT_ENOUGH_STACK_DATA;                                                                     \
        }                                                                                                              \
        type op1, op2;                                                                                                 \
        bmcl_memwriter_pop(&interp->stack, &op1, sizeof(type));                                                        \
        bmcl_memwriter_pop(&interp->stack, &op2, sizeof(type));                                                        \
        type result = op1 op op2;                                                                                      \
        bmcl_memwriter_write(&interp->stack, &result, sizeof(type));                                                   \
        return BMCL_SUCCESS;                                                                                           \
    }

MAKE_MATH_FUNCTION(int8_t, add, i8, +);
MAKE_MATH_FUNCTION(int16_t, add, i16, +);
MAKE_MATH_FUNCTION(int32_t, add, i32, +);
MAKE_MATH_FUNCTION(int64_t, add, i64, +);
MAKE_MATH_FUNCTION(uint8_t, add, u8, +);
MAKE_MATH_FUNCTION(uint16_t, add, u16, +);
MAKE_MATH_FUNCTION(uint32_t, add, u32, +);
MAKE_MATH_FUNCTION(uint64_t, add, u64, +);
MAKE_MATH_FUNCTION(float, add, float, +);
MAKE_MATH_FUNCTION(double, add, double, +);

MAKE_MATH_FUNCTION(int8_t, sub, i8, -);
MAKE_MATH_FUNCTION(int16_t, sub, i16, -);
MAKE_MATH_FUNCTION(int32_t, sub, i32, -);
MAKE_MATH_FUNCTION(int64_t, sub, i64, -);
MAKE_MATH_FUNCTION(uint8_t, sub, u8, -);
MAKE_MATH_FUNCTION(uint16_t, sub, u16, -);
MAKE_MATH_FUNCTION(uint32_t, sub, u32, -);
MAKE_MATH_FUNCTION(uint64_t, sub, u64, -);
MAKE_MATH_FUNCTION(float, sub, float, -);
MAKE_MATH_FUNCTION(double, sub, double, -);

MAKE_MATH_FUNCTION(int8_t, mult, i8, *);
MAKE_MATH_FUNCTION(int16_t, mult, i16, *);
MAKE_MATH_FUNCTION(int32_t, mult, i32, *);
MAKE_MATH_FUNCTION(int64_t, mult, i64, *);
MAKE_MATH_FUNCTION(uint8_t, mult, u8, *);
MAKE_MATH_FUNCTION(uint16_t, mult, u16, *);
MAKE_MATH_FUNCTION(uint32_t, mult, u32, *);
MAKE_MATH_FUNCTION(uint64_t, mult, u64, *);
MAKE_MATH_FUNCTION(float, mult, float, *);
MAKE_MATH_FUNCTION(double, mult, double, *);

MAKE_MATH_FUNCTION(int8_t, div, i8, /);
MAKE_MATH_FUNCTION(int16_t, div, i16, /);
MAKE_MATH_FUNCTION(int32_t, div, i32, /);
MAKE_MATH_FUNCTION(int64_t, div, i64, /);
MAKE_MATH_FUNCTION(uint8_t, div, u8, /);
MAKE_MATH_FUNCTION(uint16_t, div, u16, /);
MAKE_MATH_FUNCTION(uint32_t, div, u32, /);
MAKE_MATH_FUNCTION(uint64_t, div, u64, /);
MAKE_MATH_FUNCTION(float, div, float, /);
MAKE_MATH_FUNCTION(double, div, double, /);

void bmcl_sci_interp_init(bmcl_sci_interp_t* self, const void* bytecode, size_t bytecode_size, void* stack,
                          size_t stack_size)
{
    bmcl_memreader_init(&self->bytecode, bytecode, bytecode_size);
    bmcl_memwriter_init(&self->stack, stack, stack_size);
}

#if BMCL_HAVE_MALLOC

bmcl_sci_interp_t* bmcl_sci_interp_create(const void* bytecode, size_t bytecode_size, size_t stack_size)
{
    bmcl_sci_interp_t* self = malloc(sizeof(bmcl_sci_interp_t) + stack_size);
    uint8_t* stack = (uint8_t*)self + sizeof(bmcl_sci_interp_t);
    bmcl_sci_interp_init(self, bytecode, bytecode_size, stack, stack_size);
    return self;
}

void bmcl_sci_interp_destroy(bmcl_sci_interp_t* self)
{
    free(self);
}

#endif

static bmcl_status_t (*jump_table[])(bmcl_sci_interp_t*) = {
    stack_push8,
    stack_push16,
    stack_push32,
    stack_push64,
    // float, double?
    convert_i8_to_i32,
    convert_i8_to_u32,
    convert_u8_to_i32,
    convert_u8_to_u32,
    convert_i16_to_i32,
    convert_i16_to_u32,
    convert_u16_to_i32,
    convert_u16_to_u32,
    convert_i32_to_i8,
    convert_i32_to_u8,
    convert_i32_to_i16,
    convert_i32_to_u16,
    convert_i32_to_i64,
    convert_i32_to_u64,
    convert_i32_to_float,
    convert_i32_to_double,
    convert_u32_to_i8,
    convert_u32_to_u8,
    convert_u32_to_i16,
    convert_u32_to_u16,
    convert_u32_to_i64,
    convert_u32_to_u64,
    convert_u32_to_float,
    convert_u32_to_double,
    convert_i64_to_i32,
    convert_i64_to_u32,
    convert_i64_to_float,
    convert_i64_to_double,
    convert_u64_to_i32,
    convert_u64_to_u32,
    convert_u64_to_float,
    convert_u64_to_double,
    convert_float_to_i32,
    convert_float_to_u32,
    convert_float_to_i64,
    convert_float_to_u64,
    convert_float_to_double,
    convert_double_to_i32,
    convert_double_to_u32,
    convert_double_to_i64,
    convert_double_to_u64,
    convert_double_to_float,
    add_i8,
    add_i16,
    add_i32,
    add_i64,
    add_u8,
    add_u16,
    add_u32,
    add_u64,
    add_float,
    add_double,
    sub_i8,
    sub_i16,
    sub_i32,
    sub_i64,
    sub_u8,
    sub_u16,
    sub_u32,
    sub_u64,
    sub_float,
    sub_double,
    mult_i8,
    mult_i16,
    mult_i32,
    mult_i64,
    mult_u8,
    mult_u16,
    mult_u32,
    mult_u64,
    mult_float,
    mult_double,
    div_i8,
    div_i16,
    div_i32,
    div_i64,
    div_u8,
    div_u16,
    div_u32,
    div_u64,
    div_float,
    div_double,
};

bmcl_status_t bmcl_sci_interp_exec_next(bmcl_sci_interp_t* interp)
{
    if (bmcl_memreader_is_empty(&interp->bytecode)) {
        return BMCL_ERR_BUFFER_OVERFLOW; //TODO: eof
    }

    uint8_t instr_id = bmcl_memreader_read_uint8(&interp->bytecode);
    size_t instr_count = sizeof(jump_table) / sizeof(jump_table[0]);
    assert(instr_count <= 256);
    if (instr_id >= instr_count) {
        return BMCL_ERR_INVALID_INSTRUCTION;
    }

    return jump_table[instr_id](interp);
}

