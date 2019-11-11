SET(SOURCES
    Charset.cc
    Exception.cc
    Image.cc
    Matrix.cc
    main.cc
    Palette.cc
    read_png.cc
    SpriteSheet.cc
    TextScreen.cc
    utils.cc
)

ADD_EXECUTABLE(gfx-convert ${SOURCES})
TARGET_LINK_LIBRARIES(gfx-convert ${PNG_LIBRARIES})
INSTALL(TARGETS gfx-convert RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
