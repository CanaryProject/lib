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

#ifndef CANARY_LIB_PROTOCOL_FLATBUFFERS_WRAPPER_H
#define CANARY_LIB_PROTOCOL_FLATBUFFERS_WRAPPER_H

#include "messages/index.hpp"
#include "general.hpp"

#include "../crypt/xtea.hpp"
#include "../pch.hpp"

namespace CanaryLib {
  class NetworkMessage;
  
  static constexpr int32_t WRAPPER_HEADER_SIZE = 2;
  static constexpr int32_t WRAPPER_MAX_BODY_SIZE = NETWORKMESSAGE_MAXSIZE - WRAPPER_HEADER_SIZE;

  class FlatbuffersWrapper : public std::enable_shared_from_this<FlatbuffersWrapper> {
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
        return getChecksum(body(), wrapper_size);
      }

      bool isSerialized() {
        return serialized;
      }

      uint16_t size() {
        return wrapper_size;
      }

      uint16_t msgSize() {
        return message_size;
      }

      uint16_t outputSize() {
        return wrapper_size + WRAPPER_HEADER_SIZE;
      }

      void reset() {
        message_size = 0;
        wrapper_size = 0;
        serialized = false;
      }

      // Flatbuffers manipulators
      void serialize();
      void deserialize();
      const EncryptedMessage *buildEncryptedMessage();
      NetworkMessage buildRawMessage();

      // Wrapper buffer manipulators
      void copy(const uint8_t *bytes, bool isSerialized = true);
      bool write(const void *bytes, uint16_t size, bool append = false);
      uint16_t loadBufferSize(const uint8_t *buffer);
      void writeSize(uint16_t size);

      // Content manipulators
      void decryptXTEA(XTEA xtea);
      void encryptXTEA(XTEA xtea);
      uint16_t prepareXTEAEncryption();

      bool readChecksum();

      static uint32_t getChecksum(const uint8_t* data, size_t length);

    private:
      uint8_t w_buffer[NETWORKMESSAGE_MAXSIZE];
      uint16_t message_size = 0;
      uint16_t encrypted_size = 0;
      uint16_t wrapper_size = 0;

      bool encrypted = false;
      bool serialized = false;

      bool canWrite(const uint32_t size) const {
        return !serialized && size < WRAPPER_MAX_BODY_SIZE;
      };
  };
}

#endif