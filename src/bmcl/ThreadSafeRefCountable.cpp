#include "bmcl/ThreadSafeRefCountable.h"

namespace bmcl {

ThreadSafeRefCountableBase::ThreadSafeRefCountableBase()
    : _rc(0)
{
}

ThreadSafeRefCountableBase::~ThreadSafeRefCountableBase()
{
}

void bmclRcAddRef(const bmcl::ThreadSafeRefCountableBase* rc)
{
    rc->_rc.fetch_add(1, std::memory_order_relaxed);
}

void bmclRcRelease(const bmcl::ThreadSafeRefCountableBase* rc)
{
    if (rc->_rc.fetch_sub(1, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete rc;
    }
}
}
