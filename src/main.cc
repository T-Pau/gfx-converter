/*
  main.cc -- main program
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

#include <iostream>

#include "Bitmap.h"
#include "Commandline.h"
#include "Exception.h"
#include "read.h"
#include "write_png.h"
#include "Noter.h"
#include "TextScreen.h"
#include "SpriteSheet.h"
#include "utils.h"

enum Format {
    FORMAT_BITMAP,
    FORMAT_TEXT,
    FORMAT_SPRITES,
    FORMAT_CHARSET,
    FORMAT_SCREEN,
    FORMAT_RAW,
    FORMAT_NOTER,
    FORMAT_PRINTFOX,
    FORMAT_RAW_CHARSET,
    FORMAT_SPECTRUM
};

std::vector<Commandline::Option> options = {
        Commandline::Option("background", 'b', "index", "specify index of background color , or 'transparent'")
};

int main(int argc, char **argv) {
    auto commandline = Commandline(options, "format image filename-prefix", "gfx-converter by Dieter Baron",
    "Report bugs to <gfx-converter@tpau.group>.",
            "Copyright (C) 1999-2022 Dieter Baron");

    auto arguments = commandline.parse(argc, argv);
    
    if (arguments.arguments.size() < 3) {
        commandline.usage(true, stderr);
        exit(1);
    }
    
    try {
        Format format;
        
        if (arguments.arguments[0] == "bitmap") {
            format = FORMAT_BITMAP;
        }
        else if (arguments.arguments[0] == "charset") {
            format = FORMAT_CHARSET;
        }
        else if (arguments.arguments[0] == "sprites") {
            format = FORMAT_SPRITES;
        }
        else if (arguments.arguments[0] == "text") {
            format = FORMAT_TEXT;
        }
        else if (arguments.arguments[0] == "raw") {
            format = FORMAT_RAW;
        }
        else if (arguments.arguments[0] == "raw-charset") {
            format = FORMAT_RAW_CHARSET;
        }
        else if (arguments.arguments[0] == "noter") {
            format = FORMAT_NOTER;
        }
        else if (arguments.arguments[0] == "printfox") {
            format = FORMAT_PRINTFOX;
        }
        else if (arguments.arguments[0] == "screen") {
            format = FORMAT_SCREEN;
        }
        else if (arguments.arguments[0] == "spectrum") {
            format = FORMAT_SPECTRUM;
        }
        else {
            throw Exception("unknown format '%s'", arguments.arguments[0].c_str());
        }

        std::shared_ptr<Image> image;
        std::optional<uint8_t> background_color;
        std::optional<uint8_t> foreground_color;

        for (const auto& option : arguments.options) {
            if (option.name == "background") {
                if (option.argument == "transparent") {
                    background_color = 255;
                }
                else {
                    // TODO: error handling
                    background_color = atoi(option.argument.c_str());
                }
            }
        }

        switch (format) {
        case FORMAT_PRINTFOX:
            image = image_read_printfox(arguments.arguments[1], std::make_shared<Palette>(Palette::c64_colodore));
            break;
            
        case FORMAT_RAW:
            image = image_read_raw(arguments.arguments[1], std::make_shared<Palette>(Palette::c64_colodore), 384, 272);
            break;

        case FORMAT_RAW_CHARSET:
            image = image_read_raw_charset(arguments.arguments[1]);
            break;

        case FORMAT_SPECTRUM:
            image = image_read_png(arguments.arguments[1], std::make_shared<Palette>(Palette::zx_spectrum));
            break;

        case FORMAT_SCREEN:
            break;

        default:
            image = image_read_png(arguments.arguments[1], std::make_shared<Palette>(Palette::c64_colodore));
        }
    
        switch (format) {
            case FORMAT_TEXT: {
                auto text_screen = TextScreen(image, 0);
                text_screen.save(arguments.arguments[2]);
                break;
            }
                
            case FORMAT_SPRITES: {
                auto sprites = SpriteSheet(image, 254);
                sprites.save(arguments.arguments[2]);
                break;
            }
                
            case FORMAT_CHARSET: {
                auto bitmap = Bitmap(image, Bitmap::C64, background_color, foreground_color);
                save_file(std::string(arguments.arguments[2]), bitmap.bitmap);
                break;
            }
                
            case FORMAT_BITMAP: {
                auto bitmap = Bitmap(image, Bitmap::C64, background_color, foreground_color);
                bitmap.save(arguments.arguments[2]);
                break;
            }
                
            case FORMAT_RAW:
            case FORMAT_RAW_CHARSET:
            case FORMAT_PRINTFOX: {
                image_write_png(arguments.arguments[2], image);
                break;
            }
                
            case FORMAT_NOTER: {
                auto bitmap = Noter(image, background_color, foreground_color);
                bitmap.save(arguments.arguments[2]);
                break;
            }
            
            case FORMAT_SCREEN: {
                if (arguments.arguments.size() < 4) {
                    std::cerr << "Usage: " << argv[0] << " screen start-charset.bin complete-charset-filename image.png ...\n";
                    exit(1);
                }
                auto charset = Charset(load_file(arguments.arguments[1]));

                auto output_charset_file_name = arguments.arguments[2];

                for (auto i = 3; i < arguments.arguments.size(); i++) {
                    auto file_name = arguments.arguments[i];
                    image = image_read_png(file_name, std::make_shared<Palette>(Palette::c64_colodore));
                    auto bitmap = Bitmap(image, Bitmap::C64, background_color, foreground_color);

                    auto screen = std::vector<uint8_t>(bitmap.get_width() * bitmap.get_height());

                    for (auto y = 0; y < bitmap.get_height(); y++) {
                        for (auto x = 0; x < bitmap.get_width(); x++) {
                            auto index = charset.add(bitmap.bitmap.data() + (y * bitmap.get_width() + x) * 8);
                            screen[y * bitmap.get_width() + x] = index;
                        }
                    }
                    auto screen_file_name = file_name.substr(0, file_name.rfind('.')) + ".bin";
                    save_file(screen_file_name, screen);
                }
                charset.save(output_charset_file_name, false);
                break;
            }

            case FORMAT_SPECTRUM: {
                auto bitmap = Bitmap(image, Bitmap::SPECTRUM, background_color, foreground_color);
                std::vector<const std::vector<uint8_t>*> data;
                data.emplace_back(&bitmap.bitmap);
                data.emplace_back(&bitmap.bitmap);
                save_file(arguments.arguments[2], data);
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
