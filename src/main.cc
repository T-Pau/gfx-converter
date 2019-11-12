/*
  main.cc -- main program
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

#include <iostream>

#include "Bitmap.h"
#include "Exception.h"
#include "read_png.h"
#include "TextScreen.h"
#include "SpriteSheet.h"

enum Format {
    FORMAT_BITMAP,
    FORMAT_TEXT,
    FORMAT_SPRITES
};

int main(int argc, const char * argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " format image.png filename-prefix\n";
        exit(1);
    }
    
    try {
        Format format;
        
        if (strcmp(argv[1], "bitmap") == 0) {
            format = FORMAT_BITMAP;
        }
        else if (strcmp(argv[1], "sprites") == 0) {
            format = FORMAT_SPRITES;
        }
        else if (strcmp(argv[1], "text") == 0) {
            format = FORMAT_TEXT;
        }
        else {
            throw Exception("unknown format '%s'", argv[1]);
        }

        
        auto image = image_read_png(argv[2], std::make_shared<Palette>(Palette::c64_colodore));
    
        switch (format) {
            case FORMAT_TEXT: {
                auto text_screen = TextScreen(image, 0);
                text_screen.save(argv[3]);
                break;
            }
                
            case FORMAT_SPRITES: {
                auto sprites = SpriteSheet(image, 254);
                sprites.save(argv[3]);
                break;
            }
                
            case FORMAT_BITMAP: {
                std::optional<uint8_t> background_color;
                std::optional<uint8_t> foreground_color = 1;
                auto bitmap = Bitmap(image, background_color, foreground_color);
                bitmap.save(argv[3]);
                break;
            }
        }
    }
    catch(Exception const &ex) {
        std::cerr << argv[0] << ": " << ex.what() << "\n";
        exit(1);
    }
    
    exit(0);
}