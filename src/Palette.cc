/*
  Palette.h -- color palette
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
#include "Palette.h"

Palette Palette::c64_colodore = Palette({
    0x000000,
    0xFFFFFF,
    0x813338,
    0x75CEC8,
    0x8E3C97,
    0x56AC4D,
    0x2E2C9B,
    0xEDF171,
    0x8E5029,
    0x553800,
    0xC46C71,
    0x4A4A4A,
    0x7B7B7B,
    0xA9FF9F,
    0x706DEB,
    0xB2B2B2
});

Palette::Palette(uint8_t size, uint8_t transparent_index_) : transparent_index(transparent_index_), entries(size) { }

Palette::Palette(const std::vector<uint32_t> entries_, uint8_t transparent_index_) : transparent_index(transparent_index_), entries(entries_) { }

uint8_t Palette::lookup(uint32_t color) const {
    for (uint8_t index = 0; index < entries.size(); index++) {
        if (entries[index] == color) {
            return index;
        }
    }
    
    throw Exception("invalid color $%06x", color);
}

uint32_t Palette::get(uint8_t index) const {
    if (index > entries.size()) {
        throw Exception("palette index out of range");
    }
    
    return entries[index];
}

uint32_t& Palette::operator [](uint8_t index) {
    if (index > entries.size()) {
        throw Exception("palette index out of range");
    }
        
    return entries[index];
}
