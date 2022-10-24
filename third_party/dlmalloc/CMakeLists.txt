# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

# Using dlmalloc avoids severe performance problems when using Retro68's default
# NewPtr-based malloc implementation.
# https://github.com/autc04/Retro68/issues/185
add_library(dlmalloc OBJECT macos-dlmalloc.c)

set(dlmalloc_LIBRARY "$<TARGET_OBJECTS:dlmalloc>"
    CACHE FILEPATH "dlmalloc library"
)

set(functions
    bulk_free calloc free independent_calloc independent_comalloc mallinfo
    malloc malloc_footprint malloc_footprint_limit malloc_inspect_all
    malloc_max_footprint malloc_set_footprint_limit malloc_stats malloc_trim
    malloc_usable_size mallopt memalign posix_memalign pvalloc realloc
    realloc_in_place valloc
)
list(TRANSFORM functions PREPEND "--wrap," OUTPUT_VARIABLE link_flags)
list(PREPEND link_flags "-Wl")
list(JOIN link_flags "," link_flags)
set(dlmalloc_LINK_FLAGS "${link_flags}"
    CACHE STRING "dlmalloc link flags"
)
