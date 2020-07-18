/**
 * Canary Lib - Canary Project a free 2D game platform
 * Copyright (C) 2020  Lucas Grossi <lucas.ggrossi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef CANARY_LIB_CRYPT_XTEA_H
#define CANARY_LIB_CRYPT_XTEA_H

#include <iostream>
#include "../pch.hpp"

namespace CanaryLib {
  static constexpr uint32_t XTEA_DELTA = 0x61C88647;
  class XTEA {
    public:
      XTEA() {}

      XTEA(const XTEA&) = delete;
      XTEA& operator=(const XTEA&) = delete;

      static XTEA& getInstance() {
        static XTEA instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
      }

      bool decrypt(uint16_t length, uint8_t* buffer) const;
      void encrypt(uint16_t length, uint8_t* buffer) const;
      void setKey(const uint32_t* k) {
			  memcpy(key, k, sizeof(*k) * 4);
      };

    private:
		  uint32_t key[4] = {};
  };  
  constexpr auto XTEA = &XTEA::getInstance;
}

#endif