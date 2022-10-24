# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

# 3.18 needed for string(HEX ...)
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(hellolite
    LANGUAGES C CXX
)

option(USE_LITEHTML "Use litehtml" ON)

# Make each function and global/static variable a separate section so that the
# linker can remove unused ones later. Set globally here so that *all* files are
# compiled with these options.
add_compile_options(-fdata-sections;-ffunction-sections)

# Retro68 renames strings.h to bsdstrings.h for the benefit of developers using
# case-insensitive filesystems compiling old classic Mac OS software that
# includes <Strings.h> but this is not needed for newer classic Mac OS software
# that includes <TextUtils.h> instead of <Strings.h> and it is not ok for modern
# software that includes <strings.h>.
# https://github.com/autc04/Retro68/issues/163
set(include_dir ${CMAKE_CURRENT_BINARY_DIR}/include)
execute_process(COMMAND
    ${CMAKE_COMMAND} -E make_directory ${include_dir}
)
execute_process(COMMAND
    ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SYSTEM_PREFIX_PATH}include/bsdstrings.h
        ${include_dir}/strings.h
)
include_directories(BEFORE SYSTEM ${include_dir})

add_subdirectory(third_party)
add_subdirectory(src)
