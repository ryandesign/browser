// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#undef DLMALLOC_DEBUG
#include <MacMemory.h>
#ifdef DLMALLOC_DEBUG
#include <TextUtils.h>
#endif
#include <sys/errno.h>
#include <sys/types.h>

#define MAP_ANON 0x1000
#define MAP_FAILED ((void *)-1)

#ifdef DLMALLOC_DEBUG
StringPtr _retro68_pstrcat(StringPtr s1, StringPtr s2)
{
    StringPtr p1 = &s1[s1[0]];
    StringPtr p2 = &s2[0];
    unsigned char s2len = s2[0];
    s1[0] += s2len;
    while (s2len--)
        *++p1 = *++p2;
    return s1;
}
#endif

void *_retro68_mmap(size_t size)
{
#ifdef DLMALLOC_DEBUG
    Str31 str, size_str;
    str[0] = 0;
    NumToString(size, size_str);
    _retro68_pstrcat(str, "\pmmap ");
    _retro68_pstrcat(str, size_str);
    _retro68_pstrcat(str, "\p bytes");
#endif
    Ptr ptr = NewPtr(size);
    if (ptr)
    {
#ifdef DLMALLOC_DEBUG
        _retro68_pstrcat(str, "\p succeeded");
        DebugStr(str);
#endif
        return ptr;
    }
#ifdef DLMALLOC_DEBUG
    _retro68_pstrcat(str, "\p failed");
    DebugStr(str);
#endif
    errno = ENOMEM;
    return MAP_FAILED;
}

void *_retro68_mremap(void *addr, size_t old_size, size_t new_size, int flags)
{
    // TODO: Implement mremap using SetPtrSize?
#ifdef DLMALLOC_DEBUG
    DebugStr("\pSomeone is calling mremap! Let's implement it.");
#endif
    return (void *)~(size_t)0;
    (void)addr;
    (void)old_size;
    (void)new_size;
    (void)flags;
}

int _retro68_munmap(void *addr, size_t size)
{
    if (size > 0)
    {
#ifdef DLMALLOC_DEBUG
        Str31 str, size_str;
        str[0] = 0;
        NumToString(size, size_str);
        _retro68_pstrcat(str, "\pmunmap ");
        _retro68_pstrcat(str, size_str);
        _retro68_pstrcat(str, "\p bytes");
        DebugStr(str);
        if (size != (size_t)GetPtrSize(addr))
            DebugStr("\pDisposing Ptr with wrong size!");
#endif
        DisposePtr(addr);
        if (MemError() == 0)
            return 0;
    }
    errno = EINVAL;
    return -1;
}

#define HAVE_MMAP 1
#define HAVE_MREMAP 1
#define MMAP(size) _retro68_mmap((size))
#define MREMAP(addr, old_size, new_size, flags) _retro68_mremap((addr), (old_size), (new_size), (flags))
#define MUNMAP(addr, size) _retro68_munmap((addr), (size))
#define DIRECT_MMAP(s) MFAIL
#define LACKS_SYS_MMAN_H 1
#define MMAP_CLEARS 0
#define HAVE_MORECORE 0
#define malloc_getpagesize ((size_t)4096U)

#define USE_DL_PREFIX 1
#define dlcalloc                        __wrap_calloc
#define dlfree                          __wrap_free
#define dlmalloc                        __wrap_malloc
#define dlmemalign                      __wrap_memalign
#define dlposix_memalign                __wrap_posix_memalign
#define dlrealloc                       __wrap_realloc
#define dlrealloc_in_place              __wrap_realloc_in_place
#define dlvalloc                        __wrap_valloc
#define dlpvalloc                       __wrap_pvalloc
#define dlmallinfo                      __wrap_mallinfo
#define dlmallopt                       __wrap_mallopt
#define dlmalloc_trim                   __wrap_malloc_trim
#define dlmalloc_stats                  __wrap_malloc_stats
#define dlmalloc_usable_size            __wrap_malloc_usable_size
#define dlmalloc_footprint              __wrap_malloc_footprint
#define dlmalloc_max_footprint          __wrap_malloc_max_footprint
#define dlmalloc_footprint_limit        __wrap_malloc_footprint_limit
#define dlmalloc_set_footprint_limit    __wrap_malloc_set_footprint_limit
#define dlmalloc_inspect_all            __wrap_malloc_inspect_all
#define dlindependent_calloc            __wrap_independent_calloc
#define dlindependent_comalloc          __wrap_independent_comalloc
#define dlbulk_free                     __wrap_bulk_free

#include "malloc.c"
