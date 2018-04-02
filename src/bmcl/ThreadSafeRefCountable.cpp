#include "bmcl/ThreadSafeRefCountable.h"

namespace bmcl {

template class ThreadSafeRefCountable<std::uint8_t>;
template class ThreadSafeRefCountable<std::uint16_t>;
template class ThreadSafeRefCountable<std::uint32_t>;
template class ThreadSafeRefCountable<std::uint64_t>;
}
