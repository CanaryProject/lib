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

#include "network_message.hpp"

namespace CanaryLib {
  uint8_t NetworkMessage::readByte(const MessageOperationType opType) {
    return unsigned(read<uint8_t>(opType));
  }

  std::string NetworkMessage::readString(uint16_t stringLen, const MessageOperationType opType) {
    if (stringLen == 0) {
      stringLen = read<uint16_t>();
    }

    if (!canRead(stringLen)) {
      return std::string();
    }

    char* v = reinterpret_cast<char*>(m_buffer) + m_info.m_bufferPos; //does not break strict aliasing
    m_info.m_bufferPos += stringLen;

    if (opType == MESSAGE_OPERATION_PEEK) m_info.m_bufferPos -= stringLen + sizeof(uint16_t);

    return std::string(v, stringLen);
  };
        
  void NetworkMessage::write(const void* bytes, const size_t size, const MessageOperationType opType) {
    if (!canWrite(size)) {
      return;
    }

    memcpy(m_buffer + m_info.m_bufferPos, bytes, size);
    m_info.m_messageSize += size;

    if (opType != MESSAGE_OPERATION_PEEK) m_info.m_bufferPos += size;
  }

  void NetworkMessage::writeByte(uint8_t value, const MessageOperationType opType) {
    if (!canWrite(1)) {
      return;
    }

    m_buffer[m_info.m_bufferPos++] = value;
    m_info.m_messageSize++;

    if (opType == MESSAGE_OPERATION_PEEK) m_info.m_bufferPos--;
  }

  void NetworkMessage::writePaddingBytes(const size_t n) {
    uint8_t byte = 0x33;
    write(&byte, n, MESSAGE_OPERATION_PEEK);
  }

  void NetworkMessage::writeString(const std::string& value, const MessageOperationType opType) {
    size_t stringLen = value.length();
    if (!canWrite(stringLen + 2)) {
      return;
    }

    write<uint16_t>(stringLen);
    write(value.c_str(), stringLen, opType);

    if (opType == MESSAGE_OPERATION_PEEK) m_info.m_bufferPos -= sizeof(uint16_t);
  };

  bool NetworkMessage::decryptXTEA(ChecksumMethods_t checksumMethod) {
    uint16_t contentLength = getLength() - (checksumMethod == CHECKSUM_METHOD_NONE ? 2 : 6);
    if ((contentLength & 7) != 0) {
      return false;
    }
    uint8_t* buffer = getBuffer() + getBufferPosition();
    
    XTEA().decrypt(contentLength, buffer);

    uint16_t innerLength = read<uint16_t>();
    if (innerLength > contentLength - 2) {
      return false;
    }
    setLength(innerLength);

    return true;
  }

  void NetworkMessage::encryptXTEA() {
    // The message must be a multiple of 8
    size_t paddingBytes = getLength() & 7;
    if (paddingBytes != 0) {
      writePaddingBytes(8 - paddingBytes);
    }

    XTEA().encrypt(getLength(), getOutputBuffer());
  }

  uint32_t NetworkMessage::getChecksum(const uint8_t* data, size_t length) {
    if (length > NETWORKMESSAGE_MAXSIZE) {
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