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


#ifndef CANARY_LIB_CRYPT_RSA_H
#define CANARY_LIB_CRYPT_RSA_H

#include <gmp.h>
#include "../pch.hpp"

namespace CanaryLib {
  class RSA {
    public:
      RSA();
      ~RSA();

      // Singleton - ensures we don't accidentally copy it
      RSA(const RSA&) = delete;
      RSA& operator=(const RSA&) = delete;

      static RSA& getInstance() {
        static RSA instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
      }

      bool decrypt(char* msg) const;
      bool encrypt(unsigned char *msg, int size);
      int getSize();
      void setKey(const char* nString, const char* dString);

    private:
		  mpz_t n, d;
  };  

  constexpr auto RSA = &RSA::getInstance;
}

#endif