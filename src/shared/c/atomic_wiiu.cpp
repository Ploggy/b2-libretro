#include <shared/system.h>

#if SYSTEM_WIIU

#include <stdint.h>
#include <stddef.h>
#include <string.h>

// PowerPC doesn't have native 64-bit atomics, so we implement them with spinlocks
static volatile uint32_t g_atomic_lock = 0;

static inline void atomic_lock(void) {
    while (__sync_lock_test_and_set(&g_atomic_lock, 1)) {
        // Spin until we acquire the lock
    }
}

static inline void atomic_unlock(void) {
    __sync_lock_release(&g_atomic_lock);
}

extern "C" {

uint64_t __atomic_load_8(const volatile void *ptr, int memorder) {
    (void)memorder;
    atomic_lock();
    uint64_t result = *(const volatile uint64_t *)ptr;
    atomic_unlock();
    return result;
}

void __atomic_store_8(volatile void *ptr, uint64_t val, int memorder) {
    (void)memorder;
    atomic_lock();
    *(volatile uint64_t *)ptr = val;
    atomic_unlock();
}

uint64_t __atomic_fetch_add_8(volatile void *ptr, uint64_t val, int memorder) {
    (void)memorder;
    atomic_lock();
    uint64_t old = *(volatile uint64_t *)ptr;
    *(volatile uint64_t *)ptr = old + val;
    atomic_unlock();
    return old;
}

}

#endif