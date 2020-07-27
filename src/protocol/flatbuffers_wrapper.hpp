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
      // Initialize wrapper content from a raw message
      FlatbuffersWrapper(NetworkMessage msg) {
        content_size = msg.getLength();
        memcpy(content_buffer, msg.getOutputBuffer(), content_size);
      }
      // Initialize wrapper content from a known content buffer and size
      FlatbuffersWrapper(uint8_t *m_buffer, uint16_t m_size) {
        content_size = m_size;
        memcpy(content_buffer, m_buffer, content_size);
      }
      // Initialize wrapper content from another wrapper buffer
      FlatbuffersWrapper(uint8_t *w_buffer) {
        copy(w_buffer);
        loadFlatbuffers();
      }

      // Getters
      uint8_t *buffer() {
        return w_buffer;
      }

      uint8_t *body() {
        return w_buffer + WRAPPER_HEADER_SIZE;
      }

      uint8_t *content() {
        return content_buffer;
      }

      uint16_t contentSize() {
        return content_size;
      }

      uint16_t encryptedSize() {
        return encrypted_size;
      }

      uint16_t size() {
        return wrapper_size;
      }

      // Flatbuffers manipulators
      void buildFlatbuffers(ContentFormat format = ContentFormat_RawMessage);
      const EncryptedMessage *loadFlatbuffers();
      void toRawMessage(NetworkMessage& output);

      // Wrapper buffer manipulators
      void copy(const uint8_t *bytes);

      bool write(const void *bytes, uint16_t size);
      void writeSize(uint16_t size);

      // Content manipulators
      void decryptXTEA(XTEA xtea);
      void encryptXTEA(XTEA xtea);
      uint16_t prepareXTEAEncryption();

    private:
      // buffers
      uint8_t content_buffer[WRAPPER_MAX_BODY_SIZE];      
      uint8_t w_buffer[NETWORKMESSAGE_MAXSIZE];

      // sizes
      uint16_t encrypted_size;
      uint16_t content_size;
      uint16_t wrapper_size;

      bool canWrite(const uint32_t size) const {
        return size < WRAPPER_MAX_BODY_SIZE;
      };
  };
}