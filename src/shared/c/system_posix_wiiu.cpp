#include <shared/system.h>

#if SYSTEM_WIIU && SYSTEM_POSIX

#include <shared/system_specific.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// WiiU doesn't have backtrace support
char **GetBacktraceSymbols(void *const *array, int size) {
    (void)array;
    (void)size;
    return nullptr;
}

// WiiU doesn't have terminal width detection
int GetTerminalWidth(void) {
    return 80; // Default width
}

// Thread naming - WiiU stub
void SetCurrentThreadNamef(const char *fmt, ...) {
    (void)fmt;
}

void SetCurrentThreadNamev(const char *fmt, va_list v) {
    (void)fmt;
    (void)v;
}

void SetCurrentThreadName(const char *name) {
    (void)name;
}

void SetSetCurrentThreadNameCallback(SetCurrentThreadNameFn fn, void *context) {
    (void)fn;
    (void)context;
}

// Sleep implementation for WiiU
void SleepMS(unsigned ms) {
    // WiiU uses different sleep mechanism
    // This is a stub - you may need to use WiiU-specific APIs
    (void)ms;
}

#endif