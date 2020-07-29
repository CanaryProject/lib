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

#include "messages/index.hpp"
#include "network_message.hpp"

#include "../crypt/xtea.hpp"
#include "../pch.hpp"

namespace CanaryLib {
  static constexpr int32_t WRAPPER_HEADER_SIZE = 2;
  static constexpr int32_t WRAPPER_MAX_BODY_SIZE = NETWORKMESSAGE_MAXSIZE - WRAPPER_HEADER_SIZE;

  class FlatbuffersWrapper {
    public:
      FlatbuffersWrapper() = default;

      // Can't copy, a wrapper its unique
      XTEA& operator=(const XTEA&) = delete;

      // Getters
      uint8_t *buffer() {
        return w_buffer;
      }

      uint8_t *body() {
        return w_buffer + WRAPPER_HEADER_SIZE;
      }

      const uint32_t checksum() {
        return NetworkMessage::getChecksum(body(), wrapper_size);
      }

      bool isSerialized() {
        return serialized;
      }

      uint16_t size() {
        return wrapper_size;
      }

      // Flatbuffers manipulators
      void serialize();
      void deserialize();
      const EncryptedMessage *buildEncryptedMessage();
      NetworkMessage buildRawMessage();

      // Wrapper buffer manipulators
      void copy(const uint8_t *bytes, bool isSerialized = true);
      bool write(const void *bytes, uint16_t size, bool append = false);
      uint16_t readSize(const uint8_t *buffer);
      void writeSize(uint16_t size);

      // Content manipulators
      void decryptXTEA(XTEA xtea);
      void encryptXTEA(XTEA xtea);
      uint16_t prepareXTEAEncryption();

    private:
      uint8_t w_buffer[NETWORKMESSAGE_MAXSIZE];
      uint16_t wrapper_size = 0;
      bool serialized = false;

      bool canWrite(const uint32_t size) const {
        return !serialized && size < WRAPPER_MAX_BODY_SIZE;
      };
  };
}

/**
 * Wrapper created -> no buffer, size 0
 * Ammend raw data (networkmessage)
 *  wrapper.buffer = databuffer, size += msg_size
 * Ammend flatbuffer data
 * 
 * 
 *   
 **/