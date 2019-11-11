/*
  SpriteSheet.cc -- two dimensional array of sprites
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

#include "SpriteSheet.h"

#include "Exception.h"
#include "utils.h"

SpriteSheet::SpriteSheet(size_t r, size_t c) : rows(r), columns(c), data(std::make_unique<unsigned char[]>(rows * columns * 64)) { }

SpriteSheet::SpriteSheet(std::shared_ptr<Image> image, uint8_t background_color) : rows(image->get_height() / 21), columns(image->get_width() / 24), data(std::make_unique<unsigned char[]>(rows * columns * 64)) {
    if (image->get_width() % 24 != 0 || image->get_height() % 21 != 0) {
        throw Exception("image dimensions not multiple of sprite size");
    }

    for (size_t sheet_y = 0; sheet_y < rows; sheet_y++) {
        for (size_t sheet_x = 0; sheet_x < columns; sheet_x++) {
            uint8_t foreground_color = background_color;

            size_t offset = (sheet_y * columns + sheet_x) * 64;
            for (size_t sprite_y = 0; sprite_y < 21; sprite_y++) {
                for (size_t byte_x = 0; byte_x < 3; byte_x++) {
                    auto byte = image->get_byte(sheet_x * 24 + byte_x * 8, sheet_y * 21 + sprite_y, background_color, &foreground_color);
                    data[offset + sprite_y * 3 + byte_x] = byte;
                }
            }
            
            // TODO: store foreground color
        }
    }
}

void SpriteSheet::save(const std::string file_name) const {
    save_file(file_name, data.get(), rows * columns * 64);
}
