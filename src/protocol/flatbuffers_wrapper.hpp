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

#include "network_message.hpp"

namespace CanaryLib {
  static constexpr int32_t WRAPPER_HEADER_SIZE = 2;
  static constexpr int32_t WRAPPER_MAX_BODY_SIZE = 2000000;
  static constexpr int32_t WRAPPER_MAX_SIZE_TO_CONCAT = WRAPPER_MAX_BODY_SIZE - NETWORKMESSAGE_MAXSIZE;

  class FlatbuffersWrapper : public std::enable_shared_from_this<FlatbuffersWrapper> {
    public:
      FlatbuffersWrapper() {
        reset();
      };
      ~FlatbuffersWrapper() {
        reset();
      };

      // Can't copy, a wrapper its unique
      FlatbuffersWrapper& operator=(const FlatbuffersWrapper&) = delete;

      // Getters
      uint8_t *Buffer() {
        return w_buffer;
      }

      flatbuffers::FlatBufferBuilder& Builder() {
        if (isWriteLocked()) 
          throw std::domain_error("[FlatbuffersWrapper::Builder]: Can't access the builder of a write locked wrapper.");
        return fbb;
      }

      const EncryptedMessage* getEncryptedMessage() {
        return GetEncryptedMessage(fbb.GetBufferPointer());
      }

      bool isEncryptionEnabled() {
        return enableXteaEncryption;
      }

      void disableEncryption() {
        enableXteaEncryption = false;
      }
      
      bool isWriteLocked() {
        return lockBuffersWrite;
      }

      uint16_t Size() {
        return w_size;
      }

      std::vector<uint8_t> Types() {
        return types;
      }

      std::vector<flatbuffers::Offset<void>> Contents() {
        return contents;
      }

      void addRawMessage(NetworkMessage& _msg) {
        if (isWriteLocked()) return;
        NetworkMessage msg;
        msg.write(_msg.getBuffer(), _msg.getLength());

        auto buffer = fbb.CreateVector(msg.getBuffer(), msg.getLength());
        auto raw_data = CanaryLib::CreateRawData(fbb, buffer, msg.getLength());
        fbb.Finish(raw_data);
        add(raw_data.Union(), CanaryLib::DataType_RawData);

        msgQueue.emplace_back(msg);
      }

      bool add(flatbuffers::Offset<void> data, DataType type);
      void copy(const uint8_t *buffer);
      void copy(const uint8_t *buffer, uint16_t size);
      uint8_t *Finish(XTEA *xtea = nullptr);
      uint16_t loadSizeFromBuffer(const uint8_t *buffer);
      bool readChecksum();

      void reset(bool preAlignment = true);

      static uint32_t getChecksum(const uint8_t* data, size_t length);

    private:
      uint8_t w_buffer[WRAPPER_MAX_BODY_SIZE];
      uint16_t w_size;

      std::vector<uint8_t> types;
      std::vector<flatbuffers::Offset<void>> contents;

      flatbuffers::FlatBufferBuilder fbb;

      std::list<NetworkMessage> msgQueue;

      bool enableXteaEncryption = true;
      bool lockBuffersWrite = false;
  };

  class FlatbuffersWraperBalancer {
    public:
      // non-copyable
      FlatbuffersWraperBalancer(const FlatbuffersWraperBalancer&) = delete;
      FlatbuffersWraperBalancer& operator=(const FlatbuffersWraperBalancer&) = delete;

      static FlatbuffersWraperBalancer& getInstance() {
        static FlatbuffersWraperBalancer instance;
        return instance;
      }

      std::shared_ptr<FlatbuffersWrapper> getOutputWrapper(std::function<void (std::shared_ptr<FlatbuffersWrapper>)> callback) {
        if (!outputWrapper) {
          outputWrapper = std::make_shared<FlatbuffersWrapper>();
        } else if (outputWrapper->Size() > WRAPPER_MAX_SIZE_TO_CONCAT) {
          callback(outputWrapper);
          outputWrapper = std::make_shared<FlatbuffersWrapper>();
        }

        return outputWrapper;
      }

    private:
      FlatbuffersWraperBalancer() = default;
      std::shared_ptr<FlatbuffersWrapper> outputWrapper;
  };
}

/*

      void parsePlayerData (const void* msg) {
        auto value = reinterpret_cast<const CanaryLib::PlayerData *>(msg);
        // do something
      }

      void parseWeaponData (const void* msg) {
        auto value = reinterpret_cast<const CanaryLib::WeaponData *>(msg);
        // do something
      }

      void parseRawData (const void* msg) {
        auto value = reinterpret_cast<const CanaryLib::RawData *>(msg);
        // do something
      }

      void parseDataError (const void* msg) {
        spdlog::critical("Error");
      }

      std::map<CanaryLib::DataType, std::function<void (const void*)>> parseMap = {
        {CanaryLib::DataType_PlayerData, parsePlayerData},
        {CanaryLib::DataType_RawData, parseRawData},
        {CanaryLib::DataType_WeaponData, parseWeaponData},
        {CanaryLib::DataType_NONE, parseDataError}
      };

*/

#endif