/*
  Image.cc -- image pixels and palette
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

#include "Image.h"

#include "Exception.h"

Image::Image(size_t width, size_t height, std::shared_ptr<Palette> palette_) : pixels(width, height), palette(palette_) { }

uint32_t Image::get_rgb(size_t x, size_t y) {
    return palette->get(pixels.get(x, y));
}

void Image::set_rgb(size_t x, size_t y, uint32_t color) {
    pixels.set(x, y, palette->lookup(color));
}

uint8_t Image::get_byte(size_t x, size_t y, std::optional<uint8_t>& background_color, std::optional<uint8_t>& foreground_color) {
    if (x % 8 != 0) {
        throw Exception("x not multiple of 8");
    }
    
    uint8_t byte = 0;
    for (size_t bit = 0; bit < 8; bit++) {
        auto pixel = get(x + bit, y);
        
        byte <<= 1;
        if (pixel == palette->transparent_index || (background_color && pixel == background_color)) {
        }
        else if (foreground_color && pixel == foreground_color) {
            byte |= 1;
        }
        else {
            if (!background_color) {
                background_color = pixel;
            }
            else if (!foreground_color) {
                foreground_color = pixel;
                byte |= 1;
            }
            else  {
                throw Exception("color clash").set_position(x + bit, y);
            }
        }
    }
    
    return byte;
}
