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
#include "network_message.hpp"

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
    auto header = CreateHeader(fbb, checksum(), encrypted_size, message_size);
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
    const EncryptedMessage *encryptedMessage = buildEncryptedMessage();

    encrypted_size = encryptedMessage->header()->encrypted_size();
    uint16_t content_size = encryptedMessage->header()->message_size();

    if (encrypted_size > 0) {
      write(encryptedMessage->body()->data(), encrypted_size);
      message_size = content_size;
      encrypted = true;
    } else {
      write(encryptedMessage->body()->data(), content_size);
    }

    serialized = false;
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
    output.write(body(), message_size, MESSAGE_OPERATION_PEEK);
    return output;
  }

  void FlatbuffersWrapper::decryptXTEA(XTEA xtea) {
    if (serialized || !encrypted) return;
    xtea.decrypt(wrapper_size, body());
    writeSize(message_size);
    encrypted_size = 0;
    encrypted = false;
  }

  void FlatbuffersWrapper::encryptXTEA(XTEA xtea) {
    if (serialized || !encryption_enabled) return;
    encrypted_size = prepareXTEAEncryption();
    xtea.encrypt(encrypted_size, body());
    encrypted = true;
  }

  // Ensure body has multiple of 8 size (xtea needs it)
  uint16_t FlatbuffersWrapper::prepareXTEAEncryption() {
    uint8_t padding = (8 - wrapper_size % 8);
    // Validate xtea size and write padding
    if (padding < 8) {
      uint8_t byte = 0x00;
      write(&byte, padding, true);
      message_size -= padding;
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
    message_size = wrapper_size;
    memcpy(w_buffer, &wrapper_size, WRAPPER_HEADER_SIZE);
  }

  bool FlatbuffersWrapper::readChecksum() {
    auto enc_msg = buildEncryptedMessage();
    encrypted_size = enc_msg->header()->encrypted_size();
    uint16_t checksumSize = encrypted_size == 0 ? enc_msg->header()->message_size() :  encrypted_size;

    return enc_msg->header()->checksum() == getChecksum(enc_msg->body()->data(), checksumSize);
  }

  uint32_t FlatbuffersWrapper::getChecksum(const uint8_t* data, size_t length) {
    if (length > NETWORKMESSAGE_MAXSIZE) {
      return 0;
    }

    const uint16_t adler = 65521;
    #if defined(__SSE2__)
    const __m128i h16 = _mm_setr_epi16(16, 15, 14, 13, 12, 11, 10, 9);
    const __m128i h8 = _mm_setr_epi16(8, 7, 6, 5, 4, 3, 2, 1);
    const __m128i zeros = _mm_setzero_si128();
    #endif

    uint32_t a = 1, b = 0;

    while (length > 0) {
      size_t tmp = length > 5552 ? 5552 : length;
      length -= tmp;

      #if defined(__SSE2__)
      while (tmp >= 16) {
        __m128i vdata = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
        __m128i v = _mm_sad_epu8(vdata, zeros);
        __m128i v32 = _mm_add_epi32(_mm_madd_epi16(_mm_unpacklo_epi8(vdata, zeros), h16), _mm_madd_epi16(_mm_unpackhi_epi8(vdata, zeros), h8));
        v32 = _mm_add_epi32(v32, _mm_shuffle_epi32(v32, _MM_SHUFFLE(2, 3, 0, 1)));
        v32 = _mm_add_epi32(v32, _mm_shuffle_epi32(v32, _MM_SHUFFLE(0, 1, 2, 3)));
        v = _mm_add_epi32(v, _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 0, 3, 2)));
        b += (a << 4) + _mm_cvtsi128_si32(v32);
        a += _mm_cvtsi128_si32(v);

        data += 16;
        tmp -= 16;
      }

      while (tmp--) {
        a += *data++; b += a;
      }
      #else
      do {
        a += *data++;
        b += a;
      } while (--tmp);
      #endif

      a %= adler;
      b %= adler;
    }

    return (b << 16) | a;
  }

  uint8_t *FlatbuffersWrapper2::Finish(XTEA *xtea) {
    if (!encrypted_message) {
      auto types_vec = fbb.CreateVector(types);
      auto contents_vec = fbb.CreateVector(contents);

      auto content_msg = CreateContentMessage(fbb, types_vec, contents_vec);
      fbb.Finish(content_msg);

      uint16_t content_size = fbb.GetSize();
      uint8_t *content_buffer = fbb.GetBufferPointer();

      // encrypt (if requested) + checksum
      if (EncryptionEnabled() && xtea && xtea->isEnabled()) {
        xtea->encrypt(content_size, fbb.GetBufferPointer());
        encrypted = true;
      }
      uint32_t checksum = FlatbuffersWrapper::getChecksum(fbb.GetBufferPointer(), content_size);

      auto content = fbb.CreateVector(fbb.GetBufferPointer(), content_size);
      auto header = CreateHeader(fbb, checksum, content_size, content_size, encrypted);
      fbb.Finish(header);

      auto enc_message = CreateEncryptedMessage(fbb, header, content);
      fbb.Finish(enc_message);  

      encrypted_message = GetEncryptedMessage(Buffer());

      uint16_t size = Size();
      memcpy(w_buffer, &size, WRAPPER_HEADER_SIZE);
      memcpy(w_buffer + WRAPPER_HEADER_SIZE, Buffer(), size);
    }
    return w_buffer;
  }

  void FlatbuffersWrapper2::reset(bool preAlignment /*= true*/) {
    types.clear();
    contents.clear();
    encryption_enabled = true;
    encrypted = false;
    encrypted_message = nullptr;

    // Needed for xtea, a closed buffer will ALWAYS have multiple of 8 size
    fbb.Reset();
    if (preAlignment) fbb.PreAlign(WRAPPER_MAX_BODY_SIZE, 8);
  }

  bool FlatbuffersWrapper2::add(flatbuffers::Offset<void> data, DataType type) {
    if (encrypted_message) return false;
    types.emplace_back(static_cast<uint8_t>(type));
    contents.emplace_back(data);
    return true;
  }

  // Copies another raw wrapper buffer
  void FlatbuffersWrapper2::copy(const uint8_t *buffer) {
    uint16_t size = loadSizeFromBuffer(buffer);
    memcpy(w_buffer, &size, WRAPPER_HEADER_SIZE);
    copy(buffer + WRAPPER_HEADER_SIZE, size);
  }

  // Copies another raw wrapper buffer
  void FlatbuffersWrapper2::copy(const uint8_t *buffer, uint16_t size) {
    // only copies well formed multiple of 8 sized buffers
    if (size % 8) return;
    // reset without pre alignment, since its a copy
    reset(0);
    uint8_t *body = w_buffer + WRAPPER_HEADER_SIZE;
    memcpy(body, buffer, size);
    fbb.PushFlatBuffer(body, size);
    encrypted_message = GetEncryptedMessage(body);
  }

  uint16_t FlatbuffersWrapper2::loadSizeFromBuffer(const uint8_t *buffer) {
    uint16_t size;
    memcpy(&size, buffer, WRAPPER_HEADER_SIZE);
    return size;
  }

  bool FlatbuffersWrapper2::readChecksum() {
    if (!encrypted_message) return true;
    return encrypted_message->header()->checksum() == FlatbuffersWrapper::getChecksum(encrypted_message->body()->data(), encrypted_message->header()->message_size());
  }
}