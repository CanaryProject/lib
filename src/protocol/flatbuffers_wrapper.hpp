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

  class FlatbuffersWrapper {
    public:
      FlatbuffersWrapper() = default;

      static uint8_t* createFlatbuffers(NetworkMessage* msg, uint16_t size, uint16_t encrypted_size) {
        uint32_t recvChecksum = NetworkMessage::getChecksum(msg->getOutputBuffer(), msg->getLength());

        // Start flabuffer creation
        flatbuffers::FlatBufferBuilder fbb;
        auto header = CreateHeader(fbb, recvChecksum, size, encrypted_size);
        // create flatbuffer vector with the outputbuffer
        auto encrypted_bytes = fbb.CreateVector(msg->getOutputBuffer(), size);

        auto encrypted_message = CreateEncryptedMessage(fbb, header, encrypted_bytes);
        fbb.Finish(encrypted_message);

        return fbb.GetBufferPointer();
      }

      static uint16_t prepareXTEAEncryption(NetworkMessage* msg) {
          // Validate xtea size and write padding
        uint16_t size = msg->getLength();
        if ((size % 8) != 0) {
          msg->writePaddingBytes(8 - (size % 8));
        }
        msg->setBufferPosition(MAX_HEADER_SIZE);

        return msg->getLength();
      }

      static NetworkMessage readFlatbuffers(uint8_t* buffer, uint16_t size) {
        NetworkMessage msg;
        auto final = GetEncryptedMessage(buffer);

        // Read message from flatbuffer
        if (final->data()) {
          msg.write(final->data()->data(), size, MESSAGE_INCREMENT_SIZE);
        }

        return msg;
      }

      static void decryptXTEA(NetworkMessage* msg, uint16_t size) {
        CanaryLib::XTEA().decrypt(size, msg->getOutputBuffer());
      }

      static void encryptXTEA(NetworkMessage* msg, uint16_t size) {
        CanaryLib::XTEA().encrypt(size, msg->getOutputBuffer());
      }
  };
}