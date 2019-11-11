/*
  utils.cc -- utility and convenience functions
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

#include "utils.h"

#include "Exception.h"

std::shared_ptr<std::FILE> make_shared_file(std::string file_name, std::string flags) {
    auto fp = std::fopen(file_name.c_str(), flags.c_str());
    
    if (fp == NULL) {
        if (flags.find('w') != std::string::npos) {
            throw Exception("can't create '%s'", file_name.c_str()).append_system_error();
        }
        else {
            throw Exception("can't open '%s'", file_name.c_str()).append_system_error();
        }
    }
    return std::shared_ptr<std::FILE>(fp, std::fclose);
}


void save_file(const std::string file_name, const uint8_t *data, size_t length) {
    auto fp = make_shared_file(file_name, "wb");
    
    fwrite(data, length, 1, fp.get());
    
    // TODO: write error handling
}

std::string string_format(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    auto string = string_format_v(format, ap);
    va_end(ap);
    return string;
}

std::string string_format_v(const char *format, va_list ap) {
    auto size = strlen(format) + 50;
    std::string str;
    va_list ap2;
    while (1) {
        str.resize(size);
        va_copy(ap2, ap);
        int n = vsnprintf((char *)str.data(), size, format, ap2);
        va_end(ap2);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1) {
            size = n + 1;
        }
        else {
            size *= 2;
        }
    }
    return str;
}
