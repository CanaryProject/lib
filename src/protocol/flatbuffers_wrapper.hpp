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
      FlatbuffersWrapper() = delete;
      FlatbuffersWrapper(NetworkMessage* input) : msg(input) {}

      // Getters
      NetworkMessage* message() {
        return msg;
      }

      uint8_t* buffer() {
        return w_buffer;
      }

      uint8_t* body() {
        return w_buffer + WRAPPER_HEADER_SIZE;
      }

      uint16_t encryptedSize() {
        return encrypted_size;
      }

      uint16_t size() {
        return w_size;
      }

      void copy(const uint8_t* bytes) {
        uint16_t size;
        memcpy(&size, bytes, WRAPPER_HEADER_SIZE);
        write(bytes + WRAPPER_HEADER_SIZE, size);
      }

      void createFlatbuffers() {
        uint32_t recvChecksum = NetworkMessage::getChecksum(msg->getOutputBuffer(), msg->getLength());

        // Start flabuffer creation
        flatbuffers::FlatBufferBuilder fbb;
        // create flatbuffer vector with the outputbuffer
        auto encrypted_bytes = fbb.CreateVector(msg->getOutputBuffer(), msg->getLength());
        // create flatbuffer header
        auto header = CreateHeader(fbb, recvChecksum, msg->getLength(), encrypted_size);

        auto encrypted_message = CreateEncryptedMessage(fbb, header, encrypted_bytes);
        fbb.Finish(encrypted_message);

        write(fbb.GetBufferPointer(), fbb.GetSize());
      }

      void writeMessage() {
        msg->reset();
        auto final = GetEncryptedMessage(body());

        // Read message from flatbuffer
        if (final->data()) {
          msg->write(final->data()->data(), final->header()->size(), MESSAGE_INCREMENT_SIZE);
        }
      }

      void decryptXTEA() {
        XTEA().decrypt(msg->getLength(), msg->getOutputBuffer());
      }

      void encryptXTEA() {
        prepareXTEAEncryption();
        XTEA().encrypt(msg->getLength(), msg->getOutputBuffer());
        encrypted_size = msg->getLength();
      }

      uint16_t prepareXTEAEncryption() {
          // Validate xtea size and write padding
        uint16_t size = msg->getLength();
        if ((size % 8) != 0) {
          msg->writePaddingBytes(8 - (size % 8));
        }
        msg->setBufferPosition(WRAPPER_HEADER_SIZE);

        return msg->getLength();
      }

      void setMessage(NetworkMessage* input) {
        msg = input;
      }

      bool write(const void* bytes, uint16_t size) {
        if (!canWrite(size)) {
          return false;
        }
        
        memcpy(w_buffer + WRAPPER_HEADER_SIZE, bytes, size);
        writeSize(size);
        return true;
      }

      void writeSize(uint16_t size) {
        w_size = size;
        memcpy(w_buffer, &w_size, WRAPPER_HEADER_SIZE);
      }

    private:
      uint16_t encrypted_size;
      NetworkMessage* msg;
      uint8_t w_buffer[NETWORKMESSAGE_MAXSIZE];
      uint16_t w_size;
      
      bool canWrite(const uint32_t size) const {
        return size < WRAPPER_MAX_BODY_SIZE;
      };
  };
}