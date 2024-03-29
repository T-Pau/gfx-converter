/*
  Bitmap.cc -- bitmap with color information
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

#include "Bitmap.h"

#include "Exception.h"
#include "utils.h"

Bitmap::Bitmap(size_t w, size_t h, Layout layout) : width(w), height(h), layout(layout), bitmap(width * height * 8, 0), screen(width, height) {
}

Bitmap::Bitmap(const std::shared_ptr<Image>& image, Layout layout, std::optional<uint8_t> background_color, std::optional<uint8_t> foreground_color) : width(image->get_width() / 8), height(image->get_height() / 8), layout(layout), bitmap((width * height * 8), 0), screen(width, height) {
    if (image->get_width() % 8 != 0 || image->get_height() % 8 != 0) {
        throw Exception("image dimensions not multiple of 8");
    }
    if (layout == SPECTRUM && (width != 32 || height != 24)) {
        throw Exception("image dimensions for Spectrum layout must be 256x192");
    }
    
    for (size_t screen_y = 0; screen_y < height; screen_y++) {
        for (size_t screen_x = 0; screen_x < width; screen_x++) {
            std::optional<uint8_t> bg_color = background_color;
            std::optional<uint8_t> fg_color = foreground_color;
            
            uint8_t tile[8];
            
            for (size_t tile_y = 0; tile_y < 8; tile_y++) {
                tile[tile_y] = image->get_byte(screen_x * 8, screen_y * 8 + tile_y, bg_color, fg_color);
            }
            
            set_tile(screen_x, screen_y, tile, bg_color ? *bg_color : 0, fg_color ? *fg_color : 0);
        }
    }
}


void Bitmap::set_tile(size_t x, size_t y, const uint8_t tile[], uint8_t foreground_color, uint8_t background_color) {
    switch (layout) {
        case C64:
            memcpy(bitmap.data() + (y * width + x) * 8, tile, 8);
            screen.set(x, y, foreground_color | (background_color << 4));
            break;

        case SPECTRUM: {
            if ((foreground_color & 0x8) != (background_color & 0x8) && foreground_color != 0 && background_color != 0) {
                throw Exception("mixing dark and bright colors").set_position(x, y);
            }
            for (size_t tile_y = 0; tile_y < 8; tile_y++) {
                size_t part = y / 8;
                size_t part_y = y % 8;
                bitmap[part * 2048 + x + part_y * 32 + tile_y * 256] = tile[tile_y];
            }
            uint8_t bright = ((background_color & 0x8) | (foreground_color & 0x8)) ? 0x40 : 0;
            screen.set(x, y, bright | ((foreground_color & 0x7) << 3) | (background_color & 0x7));
            break;
        }

    }
}



void Bitmap::save(const std::string& file_name_prefix) const {
    save_file(file_name_prefix + "-bitmap.bin", bitmap.data(), bitmap.size());
    screen.save(file_name_prefix + "-screen.bin");
}
