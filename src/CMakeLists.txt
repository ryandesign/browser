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

set_target_properties(app PROPERTIES
    C_STANDARD 99
    CXX_STANDARD 11
)

set_target_properties(app PROPERTIES
    COMPILE_OPTIONS "-fdata-sections;-ffunction-sections"
    LINK_FLAGS "-Wl,--gc-sections"
)
