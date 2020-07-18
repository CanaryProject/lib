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
  uint8_t NetworkMessage::readByte() {
    return unsigned(read<uint8_t>());
  }

  std::string NetworkMessage::readString(uint16_t stringLen) {
    if (stringLen == 0) {
      stringLen = read<uint16_t>();
    }

    if (!canRead(stringLen)) {
      return std::string();
    }

    char* v = reinterpret_cast<char*>(m_buffer) + m_info.m_bufferPos; //does not break strict aliasing
    m_info.m_bufferPos += stringLen;
    return std::string(v, stringLen);
  };
        
  void NetworkMessage::write(const void* bytes, const size_t size, const MessageIncrementType increment) {
    if (!canWrite(size)) {
      return;
    }

    memcpy(m_buffer + m_info.m_bufferPos, bytes, size);
    if (increment != MESSAGE_INCREMENT_SIZE) m_info.m_bufferPos += size;
    if (increment != MESSAGE_INCREMENT_BUFFER) m_info.m_messageSize += size;
  }

  void NetworkMessage::writeByte(uint8_t value) {
    if (!canWrite(1)) {
      return;
    }

    m_buffer[m_info.m_bufferPos++] = value;
    m_info.m_messageSize++;
  }

  void NetworkMessage::writePaddingBytes(const size_t n) {
    uint8_t byte = 0x33;
    write(&byte, n, MESSAGE_INCREMENT_SIZE);
  }

  void NetworkMessage::writeString(const std::string& value) {
    size_t stringLen = value.length();
    if (!canWrite(stringLen + 2)) {
      return;
    }

    write<uint16_t>(stringLen);
    write(value.c_str(), stringLen);
  };

  bool NetworkMessage::decryptRSA() {
    if ((getLength() - getBufferPosition()) < 128) {
      return false;
    }

    RSA().decrypt(reinterpret_cast<char*>(getBuffer()) + getBufferPosition()); //does not break strict aliasing
    return (readByte() == 0);
  }

  bool NetworkMessage::encryptRSA() {
    int size = RSA().getSize();
    if(m_info.m_messageSize  < size) {
      return false;
    }

    return RSA().encrypt(static_cast<unsigned char*>(m_buffer) + m_info.m_bufferPos - size, size);
  }

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
}