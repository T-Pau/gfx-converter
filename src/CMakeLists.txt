SET(SOURCES
    Bitmap.cc
    Charset.cc
    Commandline.cc
    Exception.cc
    Image.cc
    Matrix.cc
    main.cc
    Noter.cc
    Palette.cc
    read_png.cc
    read_printfox.cc
    read_raw.cc
    read_raw_charset.cc
    SpriteSheet.cc
    TextScreen.cc
    utils.cc
    write_png.cc
)

ADD_EXECUTABLE(gfx-convert ${SOURCES})
TARGET_LINK_LIBRARIES(gfx-convert PRIVATE PNG::PNG)
INSTALL(TARGETS gfx-convert RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
