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
#include "flatbuffers_parser.hpp"

namespace CanaryLib {
  void FlatbuffersParser::parseEncryptedMessage(const CanaryLib::EncryptedMessage *enc_msg, XTEA *xtea) {
    auto header = enc_msg->header();
    uint8_t *body_buffer = (uint8_t *) enc_msg->body()->Data();
    
    // if non-encrypted then its first message
    if (xtea && xtea->isEnabled() && header->encrypted()) {
      xtea->decrypt(enc_msg->header()->message_size(), body_buffer);
    }

    parseContentMessage(CanaryLib::GetContentMessage(body_buffer));
  }
  
  /**
   * Controls the content message routing.
   * It will identify the type and call the proper parser method.
  */
  void FlatbuffersParser::parseContentMessage(const ContentMessage *content_msg) {
    for (int i = 0; i < content_msg->data()->size(); i++) {
      switch (auto dataType = content_msg->data_type()->GetEnum<DataType>(i)) {
        case DataType_CharactersListData:
          parseCharacterList(content_msg->data()->GetAs<CharactersListData>(i));
          break;

        case DataType_ErrorData:
          parseError(content_msg->data()->GetAs<ErrorData>(i));
          break;

        case DataType_LoginData:
          parseLogin(content_msg->data()->GetAs<LoginData>(i));
          break;

        case DataType_RawData:
          parseRawData(content_msg->data()->GetAs<RawData>(i));
          break;
        
        case DataType_NONE:
        default:
          spdlog::warn("[Protocol::parseContentMessage] Invalid {} content message data type was skipped.", dataType);
          break;
      }
    }
  }
}