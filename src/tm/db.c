#include "bmcl/tm/db.h"
#include "bmcl/core/writer.h"

#include <assert.h>

void tm_db_copy_frame(tm_db_t* self, size_t frame_num, writer_t* dest)
{
    assert(frame_num < self->frame_num);
    tm_frame_t* frame = &self->frames[frame_num];
    for (size_t i = 0; i < frame->id_num; i++) {
        uint16_t id = &frame->var_ids[i];
        tm_var_t* var = self->vars[id];
        writer_write(dest, var->ptr, var->size);
    }
}
