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
  void FlatbuffersWrapper::copy(const uint8_t *bytes, bool isSerialized) {
    write(bytes + WRAPPER_HEADER_SIZE, loadBufferSize(bytes));
    serialized = isSerialized;
  }

  // Serializes the wrapper, closing it to writing
  void FlatbuffersWrapper::serialize() {
    if (serialized) {
      throw std::domain_error("[FlatbuffersWrapper::serialized]: attempt to serialize a serialized buffer.");
    };

    flatbuffers::FlatBufferBuilder fbb;
    auto encrypted_bytes = fbb.CreateVector(body(), wrapper_size);
    auto header = CreateHeader(fbb, checksum(), wrapper_size);
    auto encrypted_message = CreateEncryptedMessage(fbb, header, encrypted_bytes);

    fbb.Finish(encrypted_message);
    write(fbb.GetBufferPointer(), fbb.GetSize());

    serialized = true;
  }

  // Deserialize the wrapper, re-opening it to writing
  void FlatbuffersWrapper::deserialize() {
    if (!serialized) {
      throw std::domain_error("[FlatbuffersWrapper::deserialized]: attempt to deserialize a deserialized buffer.");
    };
    const EncryptedMessage *encrypted = buildEncryptedMessage();
    serialized = false;
    write(encrypted->body()->data(),  encrypted->header()->size());
  }

  // Loads the content from the stored flatbuffers
  const EncryptedMessage *FlatbuffersWrapper::buildEncryptedMessage() {
    if (!serialized) {
      spdlog::warn("[FlatbuffersWrapper::buildEncryptedMessage]: Forced serialize.");
      serialize();
    }
    return GetEncryptedMessage(body());
  }

  // Writes the content into a raw message
  NetworkMessage FlatbuffersWrapper::buildRawMessage() {
    NetworkMessage output;
    if (serialized) {
      spdlog::warn("[FlatbuffersWrapper::buildEncryptedMessage]: Forced deserialize.");
      deserialize();
    }
    output.write(body(), wrapper_size, MESSAGE_OPERATION_PEEK);
    return output;
  }

  void FlatbuffersWrapper::decryptXTEA(XTEA xtea) {
    if (serialized) return;
    xtea.decrypt(wrapper_size, body());
  }

  void FlatbuffersWrapper::encryptXTEA(XTEA xtea) {
    if (serialized) return;
    prepareXTEAEncryption();
    xtea.encrypt(wrapper_size, body());
  }

  // Ensure body has multiple of 8 size (xtea needs it)
  uint16_t FlatbuffersWrapper::prepareXTEAEncryption() {
    uint8_t padding = (8 - wrapper_size % 8);
    // Validate xtea size and write padding
    if (padding < 8) {
      uint8_t byte = 0x33;
      write(&byte, padding, true);
    }
    return wrapper_size;
  }

  // Writes in the wrapper buffer body
  bool FlatbuffersWrapper::write(const void *bytes, uint16_t size, bool append) {
    uint16_t offset = !append ? 0 : wrapper_size;
    if (append && !canWrite(size)) {
      return false;
    }

    memcpy(body() + offset, bytes, size);
    writeSize(size + offset);
    return true;
  }

  // Read the size of a buffer
  uint16_t FlatbuffersWrapper::loadBufferSize(const uint8_t *buffer) {
    // read size
    uint16_t size;
    memcpy(&size, buffer, WRAPPER_HEADER_SIZE);
    writeSize(size);
    return size;
  }

  // Writes the size the wrapper buffer header
  void FlatbuffersWrapper::writeSize(uint16_t size) {
    wrapper_size = size;
    memcpy(w_buffer, &wrapper_size, WRAPPER_HEADER_SIZE);
  }
}