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

#include "rsa.hpp"

namespace CanaryLib {
  RSA::RSA() {
    mpz_init(n);
    mpz_init2(d, 1024);
  }

  RSA::~RSA() {
    mpz_clear(n);
    mpz_clear(d);
  }

  void RSA::decrypt(char* msg) const {
    mpz_t c, m;
    mpz_init2(c, 1024);
    mpz_init2(m, 1024);

    mpz_import(c, 128, 1, 1, 0, 0, msg);

    // m = c^d mod n
    mpz_powm(m, c, d, n);

    size_t count = (mpz_sizeinbase(m, 2) + 7) / 8;
    memset(msg, 0, 128 - count);
    mpz_export(msg + (128 - count), nullptr, 1, 1, 0, 0, m);

    mpz_clear(c);
    mpz_clear(m);
  }

  bool RSA::encrypt(unsigned char *msg, int size) {
    if(size != getSize())
        return false;

    mpz_t c, m;
    mpz_init(c);
    mpz_init(m);
    mpz_import(m, size, 1, 1, 0, 0, msg);

    // c = m^e mod n
    mpz_powm(c, m, d, n);

    size_t count = (mpz_sizeinbase(m, 2) + 7) / 8;
    memset((char*)msg, 0, size - count);
    mpz_export((char*)msg + (size - count), nullptr, 1, 1, 0, 0, c);

    mpz_clear(c);
    mpz_clear(m);

    return true;
  }

  int RSA::getSize() {
    size_t count = (mpz_sizeinbase(n, 2) + 7) / 8;
    return ((int)count / 128) * 128;
  }

  void RSA::setKey(const char* nString, const char* dString) {
    mpz_set_str(n, nString, 10);
    mpz_set_str(d, dString, 10);
  }
}