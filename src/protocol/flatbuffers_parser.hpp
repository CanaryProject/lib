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

#include "messages/index.hpp"
#include "general.hpp"

namespace CanaryLib {
  class FlatbuffersParser : public std::enable_shared_from_this<FlatbuffersParser> {
    public:
      FlatbuffersParser();
      void parseContentMessage(const CanaryLib::ContentMessage *content_msg);

    protected:
      virtual void parseCharacterList(const CanaryLib::CharactersListData *character_list){};
      virtual void parseError(const CanaryLib::ErrorData *error){};
      virtual void parseLogin(const CanaryLib::LoginData *login){};
      virtual void parseRawData(const CanaryLib::RawData *raw_data){};
  };
}

#endif