#include "bmcl/tm/db.h"
#include "bmcl/core/writer.h"

#include <assert.h>

static tm_db_t tm_global_db;

void tm_db_init(tm_var_t* vars, size_t var_count, tm_frame_t* frames, size_t frame_count,
                tm_frame_serializer_t serializer)
{
    tm_global_db.vars = vars;
    tm_global_db.var_count = var_count;
    tm_global_db.frames = frames;
    tm_global_db.frame_count = frame_count;
    tm_global_db.serializer = serializer;
}

bool tm_db_serialize_frame(size_t frame_num, writer_t* dest, void* user_data)
{
    return tm_global_db.serializer(frame_num, dest, user_data);
}

void tm_db_copy_frame(size_t frame_num, writer_t* dest)
{
    assert(tm_db_has_frame(frame_num));
    tm_frame_t* frame = &tm_global_db.frames[frame_num];
    for (size_t i = 0; i < frame->id_count; i++) {
        tm_var_t* var = &tm_global_db.vars[frame->var_ids[i]];
        writer_write(dest, var->ptr, var->size);
    }
}

bool tm_db_has_frame(size_t frame_num)
{
    return frame_num < tm_global_db.frame_count;
}

size_t tm_db_recalc_frame_size(size_t frame_num)
{
    assert(tm_db_has_frame(frame_num));
    size_t size = 0;
    tm_frame_t* frame = &tm_global_db.frames[frame_num];
    for (size_t i = 0; i < frame->id_count; i++) {
        size += tm_global_db.vars[frame->var_ids[i]].size;
    }
    return size;
}

size_t tm_db_frame_size(size_t frame_num)
{
    assert(tm_db_has_frame(frame_num));
    return tm_global_db.frames[frame_num].total_size;
}

size_t tm_db_frame_len(size_t frame_num)
{
    assert(tm_db_has_frame(frame_num));
    return tm_global_db.frames[frame_num].id_count;
}

size_t tm_db_frame_count()
{
    return tm_global_db.frame_count;
}

size_t tm_db_var_count()
{
    return tm_global_db.var_count;
}
