/*
  Exception.cc -- basic exception
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

#include "Exception.h"

#include "utils.h"

Exception::Exception(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    message = string_format_v(format, ap);
    va_end(ap);
    update_full_message();
}

Exception Exception::append(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    message += " " + string_format(format, ap);
    va_end(ap);
    update_full_message();

    return *this;
}

Exception Exception::append_system_error(int code) {
    if (code == -1) {
        code = errno;
    }
    
    return append(": %s", strerror(code));
}


void Exception::update_full_message() {
    if (!position_set) {
        full_message = message.c_str();
    }
    
    full_message = string_format("%s at (%zu, %zu)", message.c_str(), x, y);
}

Exception Exception::set_position(size_t x_, size_t y_) {
    position_set = true;
    x = x_;
    y = y_;
    update_full_message();

    return *this;
}

Exception Exception::offset_position(size_t x_offset, size_t y_offset) {
    if (position_set) {
        x += x_offset;
        y += y_offset;
    }
    update_full_message();

    return *this;
}
