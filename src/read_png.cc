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

#include "read.h"

#include <cstring>

#include <string.h>

#include <png.h>

#include "Exception.h"
#include "utils.h"


std::shared_ptr<Image> image_read_png(const std::string file_name, std::shared_ptr<Palette> palette) {
    auto fp = make_shared_file(file_name, "rb");
    
    uint8_t header[8];

    if (fread(header, 8, 1, fp.get()) != 1) {
        throw Exception("can't read PNG header from '%s'", file_name.c_str()).append_system_error();
    }

    if (png_sig_cmp(header, 0, 8) != 0) {
        throw Exception("'%s' is not a PNG image", file_name.c_str());
    }
    
    auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL) {
        throw Exception("can't create PNG reader");
    }

    auto info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == NULL) {
        throw Exception("can't create PNG info");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        throw Exception("can't read PNG image '%s'", file_name.c_str()); // TODO: error details
    }

    png_init_io(png_ptr, fp.get());
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    
    auto width = png_get_image_width(png_ptr, info_ptr);
    auto height = png_get_image_height(png_ptr, info_ptr);
    auto color_type = png_get_color_type(png_ptr, info_ptr);
    auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    else if (color_type == PNG_COLOR_TYPE_GRAY) {
        png_set_gray_to_rgb(png_ptr);
    }
    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        //png_set_invert_alpha(png_ptr);
    }
    else {
        png_set_filler(png_ptr, 255, PNG_FILLER_AFTER);
    }

    png_read_update_info(png_ptr, info_ptr);

    auto image = std::make_shared<Image>(width, height, palette);
    
    if (png_get_rowbytes(png_ptr, info_ptr) != width * 4) {
        throw Exception("unexpected row size %zu", png_get_rowbytes(png_ptr, info_ptr));
    }
    uint8_t buffer[width * height * 4];
    uint8_t *rows[height];
    
    for (size_t i = 0; i < height; i++) {
        rows[i] = buffer + i * width * 4;
    }
    
    png_read_image(png_ptr, reinterpret_cast<png_bytepp>(rows));
    
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            uint32_t pixel_rgb = (rows[y][x * 4] << 16) | (rows[y][x * 4 + 1] << 8) | (rows[y][x * 4 + 2]);
            auto alpha = rows[y][x * 4 + 3];
            
            try {
                if (alpha == 255) {
                    image->set(x, y, palette->lookup(pixel_rgb));
                }
                else if (alpha == 0) {
                    image->set(x, y, palette->transparent_index);
                }
                else {
                    throw Exception("invalid alpha value %u", alpha);
                }
            }
            catch(Exception &ex) {
                throw ex.set_position(x, y);
            }
        }
    }

    return image;
}
