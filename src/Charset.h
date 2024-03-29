/*
  Charset.cc -- set of 8x8 tiles
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

#ifndef HAD_CHARSET_H
#define HAD_CHARSET_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Charset {
public:
    explicit Charset(size_t max_chars = 256);
    explicit Charset(std::vector<uint8_t> data, size_t max_chars = 256);

    size_t add(const uint8_t *tile);
    std::optional<size_t> find(const uint8_t *tile);
    
    void save(const std::string& file_name, bool full = false) const;
    
private:
    std::vector<uint8_t> data;
    size_t nchars;
    size_t max_chars;
    
    std::unordered_map<uint64_t, size_t> chars;
};

#endif // HAD_CHARSET_H
