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

namespace CanaryLib {
  class FlatbuffersParser {
    public:
      FlatbuffersParser(){};
      void parseEncryptedMessage(const CanaryLib::EncryptedMessage *enc_msg, XTEA *xtea);
      void parseContentMessage(const ContentMessage *content_msg);

    protected:
      virtual void parseCharacterList(const CharactersListData *character_list){};
      virtual void parseError(const ErrorData *error){};
      virtual void parseLogin(const LoginData *login){};
      virtual void parseRawData(const RawData *raw_data){};
  };
}

#endif