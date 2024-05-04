/*
  Exception.h -- basic exception
  Copyright (C) Dieter Baron

  This file is part of gfx-convert, a graphics converter toolbox
  for 8-bit systems.
  The authors can be contacted at <gfx-convert@tpau.group>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. The names of the authors may not be used to endorse or promote
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

#ifndef HAD_EXCEPTION_H
#define HAD_EXCEPTION_H

#include <cstring>
#include <exception>
#include <string>

class Exception : std::exception {
public:
    explicit Exception(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
    
    [[nodiscard]] const char* what() const noexcept override { return full_message.c_str(); }
    
    Exception append(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
    Exception append_system_error(int code = -1);
    Exception set_position(size_t x, size_t y);
    Exception offset_position(size_t x_offset, size_t y_offset);

private:
    void update_full_message();

    std::string message;
    std::string full_message;
    bool position_set{false};
    size_t x{};
    size_t y{};
};

#endif // HAD_EXCEPTION_H
