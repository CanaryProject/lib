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

#include "flatbuffers_wrapper.hpp"
#include "network_message.hpp"

namespace CanaryLib {
  uint8_t *FlatbuffersWrapper::Finish(XTEA *xtea) {
    if (!isWriteLocked()) {
      lockBuffersWrite = true;
      auto types_vec = fbb.CreateVector(types);
      auto contents_vec = fbb.CreateVector(contents);

      auto content_msg = CreateContentMessage(fbb, types_vec, contents_vec);
      fbb.Finish(content_msg);

      uint16_t content_size = fbb.GetSize();
      uint8_t *content_buffer = fbb.GetBufferPointer();

      bool encrypted = false;
      // encrypt (if requested) + checksum
      if (isEncryptionEnabled() && xtea && xtea->isEnabled()) {
        xtea->encrypt(content_size, content_buffer);
        encrypted = true;
      }
      uint32_t checksum = FlatbuffersWrapper::getChecksum(content_buffer, content_size);
      auto content = fbb.CreateVector(content_buffer, content_size);
      auto header = CreateHeader(fbb, checksum, content_size, content_size, encrypted);
      fbb.Finish(header);

      auto enc_message = CreateEncryptedMessage(fbb, header, content);
      fbb.Finish(enc_message);  

      spdlog::critical("encode {} {}", readChecksum(), FlatbuffersWrapper::getChecksum(content_buffer, content_size));
      w_size = fbb.GetSize();
      memcpy(w_buffer, &w_size, WRAPPER_HEADER_SIZE);
      memcpy(w_buffer + WRAPPER_HEADER_SIZE, fbb.GetBufferPointer(), w_size);
    }
    return w_buffer;
  }

  void FlatbuffersWrapper::reset(bool preAlignment /*= true*/) {
    w_size = 0;
    types.clear();
    contents.clear();
    msgQueue.clear();

    enableXteaEncryption = true;
    lockBuffersWrite = false;

    // Needed for xtea, a closed buffer will ALWAYS have multiple of 8 size
    fbb.Reset();
    if (preAlignment) fbb.PreAlign(WRAPPER_MAX_BODY_SIZE, 8);
  }

  bool FlatbuffersWrapper::add(flatbuffers::Offset<void> data, DataType type) {
    if (isWriteLocked()) return false;
    types.emplace_back(static_cast<uint8_t>(type));
    contents.emplace_back(data);
    w_size = fbb.GetSize();
    return true;
  }

  // Copies another raw wrapper buffer
  void FlatbuffersWrapper::copy(const uint8_t *buffer) {
    loadSizeFromBuffer(buffer);
    memcpy(w_buffer, &w_size, WRAPPER_HEADER_SIZE);
    copy(buffer + WRAPPER_HEADER_SIZE, w_size);
  }

  // Copies another raw wrapper buffer
  void FlatbuffersWrapper::copy(const uint8_t *buffer, uint16_t size) {
    // only copies well formed multiple of 8 sized buffers
    if (size % 8) return;
    // reset without pre alignment, since its a copy
    reset(false);
    w_size = size;
    lockBuffersWrite = true;

    uint8_t *body = w_buffer + WRAPPER_HEADER_SIZE;
    memcpy(body, buffer, size);
    fbb.PushFlatBuffer(body, size);
  }

  uint16_t FlatbuffersWrapper::loadSizeFromBuffer(const uint8_t *buffer) {
    memcpy(&w_size, buffer, WRAPPER_HEADER_SIZE);
    return w_size;
  }

  bool FlatbuffersWrapper::readChecksum() {
    if (!isWriteLocked()) return true;
    spdlog::critical("{} {}", getEncryptedMessage()->header()->checksum(), FlatbuffersWrapper::getChecksum(getEncryptedMessage()->body()->data(), getEncryptedMessage()->header()->message_size()));
    return getEncryptedMessage()->header()->checksum() == FlatbuffersWrapper::getChecksum(getEncryptedMessage()->body()->data(), getEncryptedMessage()->header()->message_size());
  }

  uint32_t FlatbuffersWrapper::getChecksum(const uint8_t* data, size_t length) {
    if (length > WRAPPER_MAX_BODY_SIZE) {
      return 0;
    }

    const uint16_t adler = 65521;
    #if defined(__SSE2__)
    const __m128i h16 = _mm_setr_epi16(16, 15, 14, 13, 12, 11, 10, 9);
    const __m128i h8 = _mm_setr_epi16(8, 7, 6, 5, 4, 3, 2, 1);
    const __m128i zeros = _mm_setzero_si128();
    #endif

    uint32_t a = 1, b = 0;

    while (length > 0) {
      size_t tmp = length > 5552 ? 5552 : length;
      length -= tmp;

      #if defined(__SSE2__)
      while (tmp >= 16) {
        __m128i vdata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
        __m128i v = _mm_sad_epu8(vdata, zeros);
        __m128i v32 = _mm_add_epi32(_mm_madd_epi16(_mm_unpacklo_epi8(vdata, zeros), h16), _mm_madd_epi16(_mm_unpackhi_epi8(vdata, zeros), h8));
        v32 = _mm_add_epi32(v32, _mm_shuffle_epi32(v32, _MM_SHUFFLE(2, 3, 0, 1)));
        v32 = _mm_add_epi32(v32, _mm_shuffle_epi32(v32, _MM_SHUFFLE(0, 1, 2, 3)));
        v = _mm_add_epi32(v, _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 0, 3, 2)));
        b += (a << 4) + _mm_cvtsi128_si32(v32);
        a += _mm_cvtsi128_si32(v);

        data += 16;
        tmp -= 16;
      }

      while (tmp--) {
        a += *data++; b += a;
      }
      #else
      do {
        a += *data++;
        b += a;
      } while (--tmp);
      #endif

      a %= adler;
      b %= adler;
    }

    return (b << 16) | a;
  }
}