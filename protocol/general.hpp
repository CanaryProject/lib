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

#ifndef CANARY_LIB_PROTOCOL_GENERAL_H
#define CANARY_LIB_PROTOCOL_GENERAL_H

#include "../pch.hpp"

namespace CanaryLib {
  /*****************************
  ********* Consts
  *****************************/
  static constexpr int32_t NETWORKMESSAGE_MAXSIZE = 65536;
  static constexpr int32_t MAX_STRING_LENGTH = 65536;
  
  using MsgSize_t = uint16_t;

  // Headers:
  // 2 bytes for unencrypted message size
  // 4 bytes for checksum
  // 2 bytes for encrypted message size
  static constexpr uint8_t MAX_HEADER_SIZE = 8;
  static constexpr uint8_t HEADER_LENGTH = 2;
  static constexpr uint8_t CHECKSUM_LENGTH = 4;
  static constexpr uint8_t XTEA_MULTIPLE = 8;
  static constexpr uint8_t NON_BODY_LENGTH =
    HEADER_LENGTH + CHECKSUM_LENGTH + XTEA_MULTIPLE;
  static constexpr uint32_t MAX_BODY_LENGTH = 
    NETWORKMESSAGE_MAXSIZE - NON_BODY_LENGTH;
  static constexpr uint32_t MAX_PROTOCOL_BODY_LENGTH = 
    MAX_BODY_LENGTH - XTEA_MULTIPLE;

/*****************************
********* Enums
*****************************/
	enum LoginServerOpts {
		LoginServerError = 0xA,
		LoginServerMotd = 0x14,
		LoginServerUpdateNeeded = 0x1E,
		LoginServerCharacterList = 0x64
	};

	enum ItemOpcode {
		StaticText = 0x60,
		UnknownCreature = 0x61,
		OutdatedCreature = 0x62,
		Creature = 0x63
	};

  struct NetworkMessageInfo {
    MsgSize_t m_messageSize = 0;
    MsgSize_t m_headerPos = MAX_HEADER_SIZE;
    MsgSize_t m_bufferPos = MAX_HEADER_SIZE;
    bool overflow = false;
  };

  class NetworkMessage {
    public:
      NetworkMessage() = default;

        /**
         *  Getters/Setters
         **/
      const uint8_t* getBuffer() const { 
        return m_buffer; 
      }
      void setBuffer(const std::string& buffer);

      uint8_t* getBody() {
        m_info.m_messageSize = HEADER_LENGTH;
        return m_buffer + HEADER_LENGTH;
      }

      const MsgSize_t getHeaderPosition() const { 
        return m_info.m_headerPos;
      }
      void setHeaderPosition(const MsgSize_t pos) {
        m_info.m_headerPos = pos;
      }

      const MsgSize_t getBufferPosition() const { 
        return m_info.m_bufferPos;
      }
      void setBufferPosition(const MsgSize_t pos) {
        m_info.m_bufferPos = pos;
      }

      const MsgSize_t getHeaderSize() const {
        return static_cast<MsgSize_t>(m_buffer[0] | m_buffer[1] << 8);
      }

      NetworkMessageInfo getMessageInfo() const { 
        return m_info; 
      }
      void setMessageInfo(const NetworkMessageInfo info) { 
        m_info = info;
      }

      const MsgSize_t getMessageSize() const { 
        return m_info.m_messageSize; 
      }
      void setMessageSize(const MsgSize_t size) { 
        m_info.m_messageSize = size; 
      }

      const bool hasOverflow() const {
        return m_info.overflow;
      }

        /**
         *  Message info manipulators
         **/
      template<typename T>
      T read() {
        if (!canRead(sizeof(T))) {
          return 0;
        }

        T value;
        memcpy(&value, m_buffer + m_info.m_bufferPos, sizeof(T));
        m_info.m_bufferPos += sizeof(T);
        return value;
      }

      uint8_t readU8() {
        return read<uint8_t>();
      }

      std::string readString(uint16_t stringLen = 0);
      
      void write(const void* bytes, size_t size) {
        if (!canWrite(size)) {
          return;
        }

        memcpy(m_buffer + m_info.m_bufferPos, bytes, size);
        m_info.m_bufferPos += size;
        m_info.m_messageSize += size;
      }

      template<typename T>
      void write(T value) {
        write(&value, sizeof(T));
      }

      void writePaddingBytes(size_t n) {
        #define canAdd(size) ((size + m_info.m_bufferPos) < NETWORKMESSAGE_MAXSIZE)
        if (!canAdd(n)) {
          return;
        }
        #undef canAdd

        memset(m_buffer + m_info.m_bufferPos, 0x33, n);
        m_info.m_messageSize += n;
      }

      void writeString(const std::string& value);

      void writeDouble(double value, uint8_t precision = 2);

      void writeItemId(uint16_t itemId);

      void writeU8(uint8_t byte) {
        write<uint8_t>(byte);
      } 

      void reset() {
         m_info = {}; 
      }
      
      void skipBytes(int16_t count) {
        m_info.m_bufferPos += count;
      }

    protected:
      NetworkMessageInfo m_info;
		  uint8_t m_buffer[NETWORKMESSAGE_MAXSIZE];

    private:
      bool canRead(uint32_t size) {
        bool sizeOverflow = size >= (NETWORKMESSAGE_MAXSIZE - m_info.m_bufferPos);
        bool positionOverflow = (m_info.m_bufferPos + size) > (m_info.m_messageSize + MAX_HEADER_SIZE);
        bool overflow = sizeOverflow || positionOverflow;

        m_info.overflow = overflow;
        return !overflow;
      };
      
      bool canWrite(uint32_t size) {
        return (size + m_info.m_bufferPos) < MAX_BODY_LENGTH;
      };
  };
}

#endif