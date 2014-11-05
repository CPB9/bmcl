#include "bmcl/core/status.h"

const char* bmcl_strerror(bmcl_status_t status)
{
    switch (status) {
    case BMCL_SUCCESS:
        return "Success";
    }
    return "Undefined error";
}
