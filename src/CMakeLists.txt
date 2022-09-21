set(REZ_INCLUDE_PATH "${hellolite_SOURCE_DIR}/include;${REZ_INCLUDE_PATH}")

add_application(app
    WebView.cpp
    hellolite.c
    hellolite.r
    ${CMAKE_CURRENT_BINARY_DIR}/master.css.r
    quickdraw_container.cpp
    quickdraw_font.cpp
    TYPE "APPL"
    CREATOR "????"
)

ExternalProject_Get_Property(litehtml SOURCE_DIR)
set(css_file ${SOURCE_DIR}/include/master.css)
set(css_rez_file ${CMAKE_CURRENT_BINARY_DIR}/master.css.r)
add_custom_command(
    COMMAND ${CMAKE_COMMAND}
        -Dinfile=${css_file}
        -Doutfile=${css_rez_file}
        -Drsrc_type=TEXT
        -Drsrc_id=128
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/text_to_rez.cmake
    DEPENDS ${css_file}
    OUTPUT ${css_rez_file}
    VERBATIM
)
add_custom_target(make_css_rez DEPENDS ${css_rez_file})

add_dependencies(app litehtml make_css_rez)

target_include_directories(app PRIVATE
    ${hellolite_SOURCE_DIR}/include
)

target_include_directories(app SYSTEM PRIVATE
    ${litehtml_INCLUDE_DIR}
)

target_link_libraries(app PRIVATE
    ${litehtml_LIBRARY}
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
    COMPILE_OPTIONS "-fdata-sections;-ffunction-sections;-Werror"
    LINK_FLAGS "-Wl,--gc-sections"
)
