/*
 read_printfox.cc -- read raw charsets
 Copyright (C) 2020 Dieter Baron
 
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

#include "Exception.h"
#include "utils.h"


std::shared_ptr<Image> image_read_raw_charset(const std::string file_name) {
    auto fp = make_shared_file(file_name, "rb");

    if (file_name.substr(file_name.length() - 4) == ".prg") {
        // skip load address
        fgetc(fp.get());
        fgetc(fp.get());
    }
    
    size_t width = 32;
    
    uint8_t bitmap[0x800];
    
    auto height = fread(bitmap, 32 * 8, 8, fp.get());
    
    if (height <= 0) {
        throw Exception("can't read charset").append_system_error();
    }

    auto image = std::make_shared<Image>(width * 8, height * 8, std::make_shared<Palette>(Palette::c64_colodore));
    
    for (size_t tile = 0; tile < width * height; tile++) {
        size_t y = tile / width;
        size_t x = tile % width;
        
        for (size_t y0 = 0; y0 < 8; y0++) {
            uint8_t byte = bitmap[tile * 8 + y0];
            size_t x0 = 0;
            for (uint8_t bit = 0x80; bit != 0; bit >>= 1) {
                image->set(x * 8 + x0, y * 8 + y0, byte & bit ? 0 : 1);
                x0 += 1;
            }
        }
    }
    
    
    return image;
}
