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
  }
        
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
    uint8_t byte = 0x00;
    write(&byte, n);
  }

  void NetworkMessage::writeString(const std::string& value, const MessageOperationType opType) {
    size_t stringLen = value.length();
    if (!canWrite(stringLen + 2)) {
      return;
    }

    write<uint16_t>(stringLen);
    write(value.c_str(), stringLen, opType);

    if (opType == MESSAGE_OPERATION_PEEK) m_info.m_bufferPos -= sizeof(uint16_t);
  }

  bool NetworkMessage::decryptXTEA(XTEA xtea, ChecksumMethods_t checksumMethod) {
    uint16_t contentLength = getLength() - (checksumMethod == CHECKSUM_METHOD_NONE ? 2 : 6);
    if ((contentLength & 7) != 0) {
      return false;
    }
    
    xtea.decrypt(contentLength, m_buffer);

    uint16_t innerLength = read<uint16_t>();
    if (innerLength > contentLength - 2) {
      return false;
    }
    
    setLength(innerLength);

    return true;
  }

  void NetworkMessage::encryptXTEA(XTEA xtea) {
    // The message must be a multiple of 8
    size_t paddingBytes = getLength() & 7;
    if (paddingBytes != 0) {
      writePaddingBytes(8 - paddingBytes);
    }

    xtea.encrypt(getLength(), m_buffer);
  }
}