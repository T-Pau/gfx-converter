/*
  TextScreen.cc -- screen with charset, screen and color RAM.
  Copyright (C) 2019 Dieter Baron

  This file is part of gfx-convert, a graphics converter toolbox
  for 8-bit systems.
  The authors can be contacted at <gfx-convert@tpau.group>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. The names of the authors may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Exception.h"
#include "TextScreen.h"

TextScreen::TextScreen(size_t width, size_t height) : screen(width, height), colors(width, height) {
}

TextScreen::TextScreen(std::shared_ptr<Image> image, uint8_t background_color) : screen(image->get_width() / 8, image->get_height() / 8), colors(image->get_width() / 8, image->get_height() / 8) {
    if (image->get_width() % 8 != 0 || image->get_height() % 8 != 0) {
        throw Exception("image dimensions not multiple of 8");
    }
    
    for (size_t screen_y = 0; screen_y < get_height(); screen_y++) {
        for (size_t screen_x = 0; screen_x < get_width(); screen_x++) {
            uint8_t foreground_color = background_color;
            uint8_t tile[8];

            for (size_t tile_y = 0; tile_y < 8; tile_y++) {
                auto tile_byte = image->get_byte(screen_x * 8, screen_y * 8 + tile_y, background_color, &foreground_color);
                tile[tile_y] =  tile_byte;
            }

            screen.set(screen_x, screen_y, charset.add(tile));
            if (foreground_color != background_color) {
                colors.set(screen_x, screen_y, foreground_color);
            }
        }
    }
}


void TextScreen::set(size_t x, size_t y, const uint8_t tile[], uint8_t color) {
    screen.set(x, y, charset.add(tile));
    colors.set(x, y, color);
    
}

void TextScreen::save(const std::string file_name_prefix, bool full) {
    charset.save(file_name_prefix + "-charset.bin", full);
    screen.save(file_name_prefix + "-screen.bin");
    colors.save(file_name_prefix + "-colors.bin");
}

