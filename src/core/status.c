#include "bmcl/core/status.h"

const char* bmcl_strerror(bmcl_status_t status)
{
    switch (status) {
    case BMCL_SUCCESS:
        return "Success";
    case BMCL_ERR_NOT_ENOUGH_SPACE:
        return "Not enough space";
    }
    return "Undefined error";
}
