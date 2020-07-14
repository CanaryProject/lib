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

#ifndef CANARY_LIB_PROTOCOL_NETWORK_MESSAGE_H
#define CANARY_LIB_PROTOCOL_NETWORK_MESSAGE_H

#include "../pch.hpp"
#include "general.hpp"

namespace CanaryLib {
	enum MessageIncrementType {
    MESSAGE_INCREMENT_BUFFER_AND_SIZE,
    MESSAGE_INCREMENT_BUFFER,
    MESSAGE_INCREMENT_SIZE
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
      uint8_t* getBuffer() { 
        return m_buffer; 
      }
      void setBuffer(const std::string& buffer);

      uint8_t* getBody() {
        m_info.m_bufferPos = HEADER_LENGTH;
        return m_buffer + HEADER_LENGTH;
      }

      MsgSize_t getHeaderPosition() const { 
        return m_info.m_headerPos;
      }
      void setHeaderPosition(const MsgSize_t pos) {
        m_info.m_headerPos = pos;
      }

      MsgSize_t getBufferPosition() const { 
        return m_info.m_bufferPos;
      }
      void setBufferPosition(const MsgSize_t pos) {
        m_info.m_bufferPos = pos;
      }

      MsgSize_t getHeaderSize() const {
        return static_cast<MsgSize_t>(m_buffer[0] | m_buffer[1] << 8);
      }

      NetworkMessageInfo getMessageInfo() const { 
        return m_info; 
      }
      void setMessageInfo(const NetworkMessageInfo info) { 
        m_info = info;
      }

      MsgSize_t getLength() const { 
        return m_info.m_messageSize; 
      }
      void setLength(const MsgSize_t size) { 
        m_info.m_messageSize = size; 
      }

      bool hasOverflow() const {
        return m_info.overflow;
      }

        /**
         *  Message Read manipulators
         **/
      template<typename T>
      T read(size_t size = 0) {
        if (size == 0) size = sizeof(T);

        if(!canRead(size)) {
          return T();
        }

        T value;
        memcpy(&value, m_buffer + m_info.m_bufferPos, size);
        m_info.m_bufferPos += size;

        return value;
      }

      // temporary, try migrate to write
      uint8_t readByte();

      // temporary, try to migrate to write
      std::string readString(uint16_t stringLen = 0);
      
        /**
         *  Message Write manipulators
         **/
      void write(const void* bytes, const size_t size, const MessageIncrementType increment = MESSAGE_INCREMENT_BUFFER_AND_SIZE);

      template<typename T>
      void write(const T value) {
        write(&value, sizeof(T));
      }

      // temporary, migrate to write
      void writeByte(uint8_t value);

      void writePaddingBytes(const size_t n);

      // temporary, try to migrate to write
      void writeString(const std::string& value);
      
      void reset() {
         m_info = {}; 
      }
      
      void skip(const size_t size) {
        m_info.m_bufferPos += size;
      }

      template<typename T>
      void skip() {
        skip(sizeof(T));
      }
      
    protected:
      NetworkMessageInfo m_info;
		  uint8_t m_buffer[NETWORKMESSAGE_MAXSIZE];

      bool canRead(const uint32_t size) {
        bool sizeOverflow = size >= (NETWORKMESSAGE_MAXSIZE - m_info.m_bufferPos);
        bool positionOverflow = (m_info.m_bufferPos + size) > (m_info.m_messageSize + MAX_HEADER_SIZE);
        m_info.overflow = sizeOverflow || positionOverflow;
        return !m_info.overflow;
      };
      
      bool canWrite(const uint32_t size) const {
        return (size + m_info.m_bufferPos) < MAX_BODY_LENGTH;
      };
  };
}

#endif