/*
  Bitmap.h -- bitmap with color information
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

#ifndef HAD_BITMAP_H
#define HAD_BITMAP_H

#include "Image.h"
#include "Matrix.h"

class Bitmap {
public:
    enum Layout {
        C64,
        SPECTRUM
    };
    Bitmap(size_t width, size_t height, Layout layout);
    Bitmap(const std::shared_ptr<Image>& image, Layout layout, std::optional<uint8_t> background_color, std::optional<uint8_t> foreground_color);
    
    [[nodiscard]] size_t get_width() const { return width; }
    [[nodiscard]] size_t get_height() const { return height; }
    [[nodiscard]] Layout get_layout() const { return layout; }

    void set_tile(size_t x, size_t y, const uint8_t tile[], uint8_t foreground_color, uint8_t background_color);
    
    void save(const std::string& file_name_prefix) const;
    

private:
    size_t width;
    size_t height;
    Layout layout;

public:
    std::vector<uint8_t> bitmap;
    Matrix screen;
};

#endif // HAD_BITMAP_H
