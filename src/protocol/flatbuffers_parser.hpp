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

#ifndef CANARY_LIB_PROTOCOL_FLATBUFFERS_PROTOCOL_H
#define CANARY_LIB_PROTOCOL_FLATBUFFERS_PROTOCOL_H

#include "../crypt/xtea.hpp"

#include "general.hpp"
#include "messages/index.hpp"
#include "network_message.hpp"

namespace CanaryLib {
  class FlatbuffersParser {
    public:
      FlatbuffersParser(){};

      void parseEncryptedMessage(const CanaryLib::EncryptedMessage *enc_msg);
      void parseContentMessage(const ContentMessage *content_msg);

      XTEA xtea;

    protected:
      virtual void parseCharacterList(const CharactersListData *character_list) {
        spdlog::debug("Calling FlatbuffersParser::parseCharacterList");
      }
      virtual void parseError(const ErrorData *error) {
        if (!error || !error->message()) return;
        spdlog::debug("Calling FlatbuffersParser::parseError");
        spdlog::error("{}", error->message()->str());
      }
      virtual void parseLoginData(const LoginData *login) {
        spdlog::debug("Calling FlatbuffersParser::parseLoginData");
      }
      virtual void parseRawData(const RawData *raw_data);

      virtual void parseThingData(const ThingData* thing);
      virtual void parseCreatureData(const CreatureData* creature, const Position *pos) {
        spdlog::debug("Calling FlatbuffersParser::parseCreatureData");
      }
      virtual void parseItemData(const ItemData* item, const Position* pos) {
        spdlog::debug("Calling FlatbuffersParser::parseItemData");
      }

      virtual void onRecvMessage(NetworkMessage& msg) {
        spdlog::debug("Calling FlatbuffersParser::onRecvMessage");
      }
  };
}

#endif