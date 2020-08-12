/*
 read_printfox.cc -- read Printfox images
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


std::shared_ptr<Image> image_read_printfox(const std::string file_name, std::shared_ptr<Palette> palette) {
    auto fp = make_shared_file(file_name, "rb");
    
    size_t width, height;
    size_t bitmap_size = 0;
    bool doubleLength;
    
    switch (fgetc(fp.get())) {
    case 'B':
        width = 40;
        height = 25;
        bitmap_size = 8192;
        doubleLength = true;
        break;
        
    case 'G':
        width = 80;
        height = 50;
        doubleLength = true;
        break;
        
    case 'P':
        height = fgetc(fp.get());
        width = fgetc(fp.get());
        doubleLength = true;
        break;
        
    default:
        throw Exception("not a Printfox image file");
    }

    if (bitmap_size == 0) {
        bitmap_size = width * height * 8;
    }
    uint8_t bitmap[bitmap_size + width * height];
    uint8_t *color = bitmap + bitmap_size;

    int runLength = 0;
    uint8_t runByte = 0;
    
    for (size_t index = 0; index < sizeof(bitmap); index++) {
        uint8_t byte = 0;
        
        if (runLength == 0) {
            if (feof(fp.get())) {
                throw Exception("premature end of file in '%s' (%zu of %zu bytes)", file_name.c_str(), index, sizeof(bitmap));
            }
            
            byte = fgetc(fp.get());
            if (ferror(fp.get())) {
                throw Exception("read error in '%s'", file_name.c_str()).append_system_error();
            }
            
            if (byte == 0x9b) {
                if (feof(fp.get())) {
                    throw Exception("premature end of file in '%s' (%zu of %zu bytes)", file_name.c_str(), index, sizeof(bitmap));
                }
                
                runLength = fgetc(fp.get());
                if (doubleLength) {
                    runLength += fgetc(fp.get()) << 8;
                }
                runByte = fgetc(fp.get());
                
//                printf("run %d byte %02x\n", runLength, runByte);
                
                if (ferror(fp.get())) {
                    throw Exception("read error in '%s'", file_name.c_str()).append_system_error();
                }
            }
        }
        
        if (runLength > 0) {
            byte = runByte;
            runLength -= 1;
        }
        
        bitmap[index] = byte;
    }

    size_t offset = ftell(fp.get());
    fseek(fp.get(), 0, SEEK_END);
    size_t end = ftell(fp.get());
    if (end != offset) {
        throw Exception("%zu bytes of trailing data in file '%s'\n", end - offset, file_name.c_str());
    }

    auto image = std::make_shared<Image>(width * 8, height * 8, palette);
    
    for (size_t tile = 0; tile < width * height; tile++) {
        size_t y = tile / width;
        size_t x = tile % width;
        
        for (size_t y0 = 0; y0 < 8; y0++) {
            uint8_t byte = bitmap[tile * 8 + y0];
            size_t x0 = 0;
            for (uint8_t bit = 0x80; bit != 0; bit >>= 1) {
                image->set(x * 8 + x0, y * 8 + y0, byte & bit ? color[tile] >> 4 : color[tile] & 0xf);
                x0 += 1;
            }
        }
    }
    
    
    return image;
}
