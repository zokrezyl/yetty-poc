// Stub for emscripten: <sys/random.h> exists but getrandom() doesn't.
// pdfio-crypto.c detects the header and calls getrandom().
// Provide a fallback that returns -1 so pdfio falls through to /dev/urandom.
#ifdef __EMSCRIPTEN__
#include <errno.h>
#include <sys/types.h>

ssize_t getrandom(void *buf, size_t buflen, unsigned int flags) {
    (void)buf; (void)buflen; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif
