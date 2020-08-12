/*
  read_png.cc -- read PNG image into Matrix for given Palette
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

#include "write_png.h"

#include <png.h>

#include "Exception.h"
#include "utils.h"


void image_write_png(const std::string file_name, std::shared_ptr<Image> image) {
    auto fp = make_shared_file(file_name, "wb");
    
    auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL) {
        throw Exception("can't create PNG writer");
    }

    auto info_ptr = png_create_info_struct(png_ptr);
    
    if (info_ptr == NULL) {
        throw Exception("can't create PNG info");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        throw Exception("can't write PNG image '%s'", file_name.c_str()); // TODO: error details
    }

    png_init_io(png_ptr, fp.get());

    png_set_IHDR(png_ptr, info_ptr, static_cast<png_uint_32>(image->get_width()), static_cast<png_uint_32>(image->get_height()), 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    uint8_t buffer[image->get_width() * image->get_height() * 3];
    uint8_t *rows[image->get_height()];
    
    for (size_t i = 0; i < image->get_height(); i++) {
        rows[i] = buffer + i * image->get_width() * 3;
    }
    
    for (size_t y = 0; y < image->get_height(); y++) {
        for (size_t x = 0; x < image->get_width(); x++) {
            auto rgb = image->get_rgb(x, y);
            rows[y][x * 3] = rgb >> 16;
            rows[y][x * 3 + 1] = (rgb >> 8) & 0xff;
            rows[y][x * 3 + 2] = rgb & 0xff;
        }
    }
    
    png_write_image(png_ptr, rows);

    png_write_end(png_ptr, NULL);
}
