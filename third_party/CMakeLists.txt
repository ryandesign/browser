# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

include(ExternalProject)

get_directory_property(compile_options COMPILE_OPTIONS)
list(JOIN compile_options " " compile_options)

get_directory_property(include_directories INCLUDE_DIRECTORIES)
list(TRANSFORM include_directories PREPEND "-isystem")
list(JOIN include_directories " " include_directories)

if(USE_DLMALLOC)
    add_subdirectory(dlmalloc)
endif()

if(USE_LITEHTML)
    ExternalProject_Add(litehtml
        CMAKE_ARGS
            -DBUILD_TESTING=OFF
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            "-DCMAKE_C_FLAGS=${include_directories} ${compile_options} -DTYPE_BOOL"
            "-DCMAKE_CXX_FLAGS=${include_directories} ${compile_options}"
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        INSTALL_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/litehtml
    )

    ExternalProject_Get_Property(litehtml BINARY_DIR)
    ExternalProject_Get_Property(litehtml SOURCE_DIR)

    set(litehtml_INCLUDE_DIR ${SOURCE_DIR}/include
        CACHE PATH "litehtml header directory"
    )
    set(litehtml_SOURCE_DIR ${SOURCE_DIR}/src
        CACHE PATH "litehtml source directory"
    )
    set(litehtml_LIBRARY "${BINARY_DIR}/liblitehtml.a;${BINARY_DIR}/src/gumbo/libgumbo.a"
        CACHE STRING "litehtml library"
    )
endif()
