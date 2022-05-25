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

#include "Charset.h"

#include <cstring>

#include "Exception.h"
#include "utils.h"

Charset::Charset(size_t max_chars) : nchars(0), data(max_chars * 8, 0), max_chars(max_chars) {
}

Charset::Charset(std::vector<uint8_t> data_, size_t max_chars) : data(std::move(data_)), max_chars(max_chars) {
    if (data.size() % 8 != 0) {
        throw Exception("charset data not multiple of 8 bytes");
    }
    if (data.size() > max_chars * 8) {
        throw Exception("charset has more data than maximum characters");
    }

    auto had_empty = false;
    
    for (auto i = 0; i < data.size() / 8; i++) {
        auto c = reinterpret_cast<const uint64_t *>(data.data())[i];
        if (c != 0 || !had_empty) {
            if (chars.find(c) == chars.end()) {
                chars[c] = i;
            }
            nchars = i + 1;
            if (c == 0) {
                had_empty = true;
            }
        }
    }

    data.resize(max_chars * 8, 0);
}

size_t Charset::add(const uint8_t tile[]) {
    auto c = *reinterpret_cast<const uint64_t *>(tile);
    auto it = chars.find(c);
    
    if (it != chars.end()) {
        return it->second;
    }
    
    if (nchars == max_chars) {
        throw Exception("out of characters");
    }
    
    auto index = nchars;
    memcpy(data.data() + index * 8, tile, 8);
    chars[c] = index;
    nchars++;
    return index;
}

std::optional<size_t> Charset::find(const uint8_t *tile) {
    auto c = *reinterpret_cast<const uint64_t *>(tile);
    auto it = chars.find(c);
    
    if (it != chars.end()) {
        return it->second;
    }

    return {};
}


void Charset::save(const std::string& file_name, bool full) const {
    save_file(file_name, data.data(), (full ? max_chars : nchars) * 8);
}
