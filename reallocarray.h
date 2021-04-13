#ifndef REALLOCARRAY_H
#define REALLOCARRAY_H
#include <stdlib.h>
#if __GLIBC_MINOR__ > 26
#if __GLIBC__ >= 2
#define HAS_REALLOC 1
#endif
#else
#define HAS_REALLOC 0
void *reallocarray(void *ptr, size_t nmemb, size_t elemsize)
{
    return realloc(ptr, nmemb * elemsize);
}
#endif
#endif
