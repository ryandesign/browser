set(REZ_INCLUDE_PATH "${hellolite_SOURCE_DIR}/include;${REZ_INCLUDE_PATH}")

add_application(app
    WebView.cpp
    hellolite.c
    hellolite.r
    quickdraw_container.cpp
    quickdraw_font.cpp
    TYPE "APPL"
    CREATOR "????"
)

add_dependencies(app litehtml)

target_include_directories(app PRIVATE
    ${hellolite_SOURCE_DIR}/include
)

target_include_directories(app SYSTEM PRIVATE
    ${litehtml_INCLUDE_DIR}
    ${litehtml_SOURCE_DIR}
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
