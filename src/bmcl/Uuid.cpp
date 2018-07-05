#include "bmcl/Uuid.h"

namespace bmcl {

#if defined(BMCL_PLATFORM_UNIX)
    #include <uuid/uuid.h>
#elif defined(BMCL_PLATFORM_WINDOWS)
    #include <Combaseapi.h>
#elif defined(BMCL_PLATFORM_APPLE)
    #include <CoreFoundation/CFUUID.h>
#else
    #error "Unsupported platform"
#endif

Uuid Uuid::create()
{
    Uuid u;

#if defined(BMCL_PLATFORM_UNIX)
    uuid_generate(u._data.data());
#elif defined(BMCL_PLATFORM_WINDOWS)
    GUID data;
    CoCreateGuid(&data);

    u._data[0] = (data.Data1 >> 24) & 0xff;
    u._data[1] = (data.Data1 >> 16) & 0xff;
    u._data[2] = (data.Data1 >>  8) & 0xff;
    u._data[3] = (data.Data1      ) & 0xff;
    u._data[4] = (data.Data2 >>  8) & 0xff;
    u._data[5] = (data.Data2      ) & 0xff;
    u._data[6] = (data.Data3 >>  8) & 0xff;
    u._data[7] = (data.Data3      ) & 0xff;

    u._data[8]  = data.Data4[0]
    u._data[9]  = data.Data4[1]
    u._data[10] = data.Data4[2]
    u._data[11] = data.Data4[3]
    u._data[12] = data.Data4[4]
    u._data[13] = data.Data4[5]
    u._data[14] = data.Data4[6]
    u._data[15] = data.Data4[7]
#elif defined(BMCL_PLATFORM_APPLE)
    CFUUID native = CFUUIDCreate(NULL);
    CFUUIDBytes data = CFUUIDGetUUIDBytes(native);
    CFRelease(native);

    u._data[0]  = data.byte0;
    u._data[1]  = data.byte1;
    u._data[2]  = data.byte2;
    u._data[3]  = data.byte3;
    u._data[4]  = data.byte4;
    u._data[5]  = data.byte5;
    u._data[6]  = data.byte6;
    u._data[7]  = data.byte7;
    u._data[8]  = data.byte8;
    u._data[9]  = data.byte9;
    u._data[10] = data.byte10;
    u._data[11] = data.byte11;
    u._data[12] = data.byte12;
    u._data[13] = data.byte13;
    u._data[14] = data.byte14;
    u._data[15] = data.byte15;
#endif

    return u;
}

}
