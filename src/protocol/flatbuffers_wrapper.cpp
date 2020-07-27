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

#include "flatbuffers_wrapper.hpp"

namespace CanaryLib {
  // Copies another wrapper buffer
  void FlatbuffersWrapper::copy(const uint8_t *bytes) {
    uint16_t size;
    memcpy(&size, bytes, WRAPPER_HEADER_SIZE);
    write(bytes + WRAPPER_HEADER_SIZE, size);
  }

  // Build the flatbuffers using the content
  void FlatbuffersWrapper::buildFlatbuffers(ContentFormat format) {
    uint32_t recvChecksum = NetworkMessage::getChecksum(content_buffer, content_size);

    // Start flabuffer creation
    flatbuffers::FlatBufferBuilder fbb;
    // create flatbuffer vector with the outputbuffer
    auto encrypted_bytes = fbb.CreateVector(content_buffer, content_size);
    // create flatbuffer header
    auto header = CreateHeader(fbb, recvChecksum, content_size, encrypted_size, format);

    auto encrypted_message = CreateEncryptedMessage(fbb, header, encrypted_bytes);
    fbb.Finish(encrypted_message);

    write(fbb.GetBufferPointer(), fbb.GetSize());
  }

  // Loads the content from the stored flatbuffers
  const EncryptedMessage *FlatbuffersWrapper::loadFlatbuffers() {
    auto encryptedMessage = GetEncryptedMessage(body());
    if (encryptedMessage->data()) {
      content_size = encryptedMessage->header()->encrypted_size();
      memcpy(content_buffer, encryptedMessage->data()->data(), content_size);
    }
    return encryptedMessage;
  }

  // Writes the content into a raw message
  void FlatbuffersWrapper::toRawMessage(NetworkMessage& output) {
    output.reset();
    output.write(content_buffer, content_size, MESSAGE_OPERATION_PEEK);
  }

  void FlatbuffersWrapper::decryptXTEA(XTEA xtea) {
    xtea.decrypt(content_size, content_buffer);
  }

  void FlatbuffersWrapper::encryptXTEA(XTEA xtea) {
    prepareXTEAEncryption();
    xtea.encrypt(content_size, content_buffer);
    encrypted_size = content_size;
  }

  // Ensure content has multiple of 8 size (xtea needs it)
  uint16_t FlatbuffersWrapper::prepareXTEAEncryption() {
    uint8_t padding = (8 - content_size % 8);
    // Validate xtea size and write padding
    if (padding < 8) {
      uint8_t byte = 0x33;
      memcpy(content_buffer + content_size, &byte, padding);
      content_size += padding;
    }

    return content_size;
  }

  // Writes in the wrapper buffer body
  bool FlatbuffersWrapper::write(const void *bytes, uint16_t size) {
    if (!canWrite(size)) {
      return false;
    }
    
    memcpy(w_buffer + WRAPPER_HEADER_SIZE, bytes, size);
    writeSize(size);
    return true;
  }

  // Writes the size the wrapper buffer header
  void FlatbuffersWrapper::writeSize(uint16_t size) {
    wrapper_size = size;
    memcpy(w_buffer, &wrapper_size, WRAPPER_HEADER_SIZE);
  }
}