/*
  Image.h -- image pixels and palette
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

#ifndef HAD_IMAGE_H
#define HAD_IMAGE_H

#include "Matrix.h"
#include "Palette.h"

class Image {
public:
    Image(size_t width, size_t height, std::shared_ptr<Palette> palette);

    size_t get_width() const { return pixels.get_width(); }
    size_t get_height() const { return pixels.get_height(); }

    uint8_t get(size_t x, size_t y) { return pixels.get(x, y); }
    void set(size_t x, size_t y, uint8_t index) { pixels.set(x, y, index); }
    
    uint32_t get_rgb(size_t x, size_t y);
    void set_rgb(size_t x, size_t y, uint32_t color);
    
    uint8_t get_byte(size_t x, size_t y, uint8_t background_color, uint8_t *foreground_color);
    
private:
    Matrix pixels;
    std::shared_ptr<Palette> palette;
};

#endif // HAD_IMAGE_H
