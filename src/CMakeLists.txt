# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

configure_file(config.h.in config.h @ONLY)

set(REZ_INCLUDE_PATH "${CMAKE_CURRENT_BINARY_DIR};${hellolite_SOURCE_DIR}/include;${REZ_INCLUDE_PATH}")

set(app_SOURCES
    about_window.cpp
    base_app.cpp
    base_control.cpp
    base_window.cpp
    browser_app.cpp
    browser_tab.cpp
    browser_window.cpp
    hellolite.r
    helpers.cpp
    machine.cpp
    main.cpp
    root_control.cpp
)

if(USE_LITEHTML)
    list(APPEND app_SOURCES
        WebView.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/home.html.r
        ${CMAKE_CURRENT_BINARY_DIR}/master.css.r
        quickdraw_container.cpp
        quickdraw_font.cpp
    )
endif()

add_application(app
    ${app_SOURCES}
    TYPE "APPL"
    CREATOR "????"
)

target_include_directories(app PRIVATE
    ${hellolite_SOURCE_DIR}/include
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL Retro68)
    target_link_libraries(app PRIVATE
        AppearanceLib
    )
endif()

set_target_properties(app PROPERTIES
    C_STANDARD 99
    CXX_STANDARD 11
)

set_target_properties(app PROPERTIES
    # TODO: Add -Wall and -Wextra eventually. Currently causes too many errors.
    COMPILE_OPTIONS "-Werror"
)
# TODO: -Wl,--gc-sections causes crashes
# https://github.com/autc04/Retro68/issues/184
# https://github.com/autc04/Retro68/issues/191
#    LINK_FLAGS "-Wl,--gc-sections"

if(USE_DLMALLOC)
    add_dependencies(app dlmalloc)

    target_link_libraries(app PRIVATE
        ${dlmalloc_LIBRARY}
    )

    set_property(TARGET app APPEND_STRING PROPERTY
        LINK_FLAGS " ${dlmalloc_LINK_FLAGS}"
    )
endif()

if(USE_LITEHTML)
    ExternalProject_Get_Property(litehtml SOURCE_DIR)

    set(text_file ${SOURCE_DIR}/include/master.css)
    set(rez_file ${CMAKE_CURRENT_BINARY_DIR}/master.css.r)
    add_custom_command(
        COMMAND ${CMAKE_COMMAND}
            -Dinfile=${text_file}
            -Doutfile=${rez_file}
            -Drsrc_type=TEXT
            -Drsrc_id=128
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/text_to_rez.cmake
        DEPENDS ${text_file}
        OUTPUT ${rez_file}
        VERBATIM
    )
    add_custom_target(make_css_rez DEPENDS ${rez_file})

    set(text_file ${CMAKE_CURRENT_SOURCE_DIR}/home.html)
    set(rez_file ${CMAKE_CURRENT_BINARY_DIR}/home.html.r)
    add_custom_command(
        COMMAND ${CMAKE_COMMAND}
            -Dinfile=${text_file}
            -Doutfile=${rez_file}
            -Drsrc_type=TEXT
            -Drsrc_id=129
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/text_to_rez.cmake
        DEPENDS ${text_file}
        OUTPUT ${rez_file}
        VERBATIM
    )
    add_custom_target(make_html_rez DEPENDS ${rez_file})

    add_dependencies(app litehtml make_css_rez make_html_rez)

    target_include_directories(app SYSTEM PRIVATE
        ${litehtml_INCLUDE_DIR}
    )

    target_link_libraries(app PRIVATE
        ${litehtml_LIBRARY}
    )

    target_compile_definitions(app PRIVATE USE_LITEHTML)
endif()
