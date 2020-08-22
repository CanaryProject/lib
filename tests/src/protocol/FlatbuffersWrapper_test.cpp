#include "../../all.h"

namespace FlatbuffersWrapperTest {
  TEST_SUITE("FlatbuffersWrapper") {
    CanaryLib::XTEA xtea;
    std::string nameStr("myName");
    std::string rawStr("rawMsg");
    uint32_t id32 = 32412516;
    uint16_t id16 = 65000;
    uint8_t dmg = 123;

    void generateBaseWrapper(CanaryLib::FlatbuffersWrapper &wrapper1) {
      xtea.generateKey();
      flatbuffers::FlatBufferBuilder &fbb = wrapper1.Builder();

      auto name = fbb.CreateString(nameStr);
      auto error_data = CanaryLib::CreateErrorData(fbb, name);
      wrapper1.add(error_data.Union(), CanaryLib::DataType_ErrorData);

      auto buffer = fbb.CreateVector((uint8_t *) rawStr.c_str(), rawStr.size());
      auto raw_data = CanaryLib::CreateRawData(fbb, buffer, rawStr.size());
      fbb.Finish(raw_data);
      wrapper1.add(raw_data.Union(), CanaryLib::DataType_RawData);
    }

    void validateBasic(CanaryLib::FlatbuffersWrapper &wrapper1, CanaryLib::XTEA* _xtea = nullptr) {
      auto enc_msg = wrapper1.getEncryptedMessage();

      CHECK_EQ(wrapper1.isWriteLocked(), true);
      CHECK(wrapper1.readChecksum());

      uint8_t *body_buffer = (uint8_t *) enc_msg->body()->Data();

      if (_xtea)
        _xtea->decrypt(enc_msg->header()->message_size(), body_buffer);

      auto content_msg = CanaryLib::GetContentMessage(body_buffer);

      CHECK_EQ(enc_msg->header()->encrypted(), !!_xtea);
      CHECK_EQ(content_msg->data()->size(), 2);

      CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(0), CanaryLib::DataType_ErrorData);
      CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(1), CanaryLib::DataType_RawData);

      auto error = content_msg->data()->GetAs<CanaryLib::ErrorData>(0);
      CHECK_EQ(error->message()->str(), nameStr);

      auto raw = content_msg->data()->GetAs<CanaryLib::RawData>(1);
      
      std::string str = std::string((char *) raw->body()->data());
      str.resize(rawStr.size());
      CHECK_EQ(str, rawStr);
      CHECK_EQ(raw->size(), rawStr.size());
    }

    TEST_CASE("Login data") {
      CanaryLib::FlatbuffersWrapper wrapper;

      flatbuffers::FlatBufferBuilder &fbb = wrapper.Builder();

      auto account = fbb.CreateString(rawStr);
      auto auth_token = fbb.CreateString(rawStr);
      auto password = fbb.CreateString(rawStr);
      auto xtea_key = fbb.CreateVector(xtea.generateKey(), 4);
      CanaryLib::LoginInfoBuilder login_info_builder(fbb);
      login_info_builder.add_account(account);
      login_info_builder.add_auth_token(auth_token);
      login_info_builder.add_password(password);
      login_info_builder.add_xtea_key(xtea_key);
      fbb.Finish(login_info_builder.Finish());

      auto releasedMsg = fbb.Release();
      auto content_size = releasedMsg.size() + sizeof(uint8_t);

      auto parsed_info = CanaryLib::GetLoginInfo(releasedMsg.data());

      uint8_t buffer[128];
      uint8_t padding = 128 - content_size;

      uint8_t byte = 0x00;
      memcpy(buffer, &byte, 1);
      memcpy(buffer + sizeof(uint8_t), releasedMsg.data(), releasedMsg.size());
      memcpy(buffer + content_size, &byte, padding);
      uint8_t final_size = content_size + padding;
      assert(final_size == 128);

      auto enc_buffer = fbb.CreateVector(buffer, final_size);

      auto challenge = CanaryLib::CreateChallenge(fbb, id32, dmg);
      auto login_data_os = CanaryLib::CreateLoginData(fbb, enc_buffer, CanaryLib::Client_t_CANARY, challenge);
      fbb.Finish(login_data_os);

      auto login_data = CanaryLib::GetLoginData(fbb.GetBufferPointer());

      CHECK_EQ(login_data->challenge()->random(), dmg);
      CHECK_EQ(login_data->challenge()->timestamp(), id32);
      CHECK_EQ(login_data->client(), CanaryLib::Client_t_CANARY);

      uint8_t buffer2[login_data->login_info()->size()];
      memcpy(buffer2, login_data->login_info()->data() + 1, login_data->login_info()->size());
      parsed_info = CanaryLib::GetLoginInfo(buffer2);

      CHECK_EQ(parsed_info->xtea_key()->size(), 4);
      CHECK_EQ(parsed_info->auth_token()->str(), rawStr);
      CHECK_EQ(parsed_info->password()->str(), rawStr);
      CHECK_EQ(parsed_info->account()->str(), rawStr);
    }

    TEST_CASE("Login Game Data") {
      CanaryLib::FlatbuffersWrapper wrapper;

      flatbuffers::FlatBufferBuilder &fbb = wrapper.Builder();

      auto char_name = fbb.CreateString(rawStr);
      auto session_key = fbb.CreateString(rawStr);
      auto game_login_info = CanaryLib::CreateGameLoginInfo(fbb, session_key, char_name);

      auto xtea_key = fbb.CreateVector(xtea.generateKey(), 4);
      CanaryLib::LoginInfoBuilder login_info_builder(fbb);
      login_info_builder.add_game_login_info(game_login_info);
      login_info_builder.add_xtea_key(xtea_key);
      fbb.Finish(login_info_builder.Finish());

      auto releasedMsg = fbb.Release();
      auto content_size = releasedMsg.size() + sizeof(uint8_t);

      auto parsed_info = CanaryLib::GetLoginInfo(releasedMsg.data());

      uint8_t buffer[128];
      uint8_t padding = 128 - content_size;

      uint8_t byte = 0x00;
      memcpy(buffer, &byte, 1);
      memcpy(buffer + sizeof(uint8_t), releasedMsg.data(), releasedMsg.size());
      memcpy(buffer + content_size, &byte, padding);
      uint8_t final_size = content_size + padding;
      assert(final_size == 128);

      auto enc_buffer = fbb.CreateVector(buffer, final_size);

      auto challenge = CanaryLib::CreateChallenge(fbb, id32, dmg);
      auto login_data_os = CanaryLib::CreateLoginData(fbb, enc_buffer, CanaryLib::Client_t_CANARY, challenge);
      fbb.Finish(login_data_os);

      auto login_data = CanaryLib::GetLoginData(fbb.GetBufferPointer());

      CHECK_EQ(login_data->challenge()->random(), dmg);
      CHECK_EQ(login_data->challenge()->timestamp(), id32);
      CHECK_EQ(login_data->client(), CanaryLib::Client_t_CANARY);

      uint8_t buffer2[login_data->login_info()->size()];
      memcpy(buffer2, login_data->login_info()->data() + 1, login_data->login_info()->size());
      parsed_info = CanaryLib::GetLoginInfo(buffer2);

      CHECK_EQ(parsed_info->game_login_info()->char_name()->str(), rawStr);
      CHECK_EQ(parsed_info->game_login_info()->session_key()->str(), rawStr);
      CHECK_EQ(parsed_info->xtea_key()->size(), 4);
    }

    TEST_CASE("Copy") {
      CanaryLib::FlatbuffersWrapper wrapper1;
      CanaryLib::FlatbuffersWrapper wrapper2;
      generateBaseWrapper(wrapper1);
      wrapper2.copy(wrapper1.Finish());
      validateBasic(wrapper2);
      CHECK_EQ(wrapper2.Size(), wrapper1.Size());
      wrapper2.copy(wrapper1.Finish() + CanaryLib::WRAPPER_HEADER_SIZE, wrapper1.Size());
      validateBasic(wrapper2);
      CHECK_EQ(wrapper2.Size(), wrapper1.Size());
    }

    TEST_CASE("Test with NetworkMessage") {
      CanaryLib::FlatbuffersWrapper wrapper;
      
      CanaryLib::NetworkMessage msg;
      flatbuffers::FlatBufferBuilder &fbb = wrapper.Builder();
      auto fbuffer = fbb.CreateVector(msg.getBuffer(), msg.getLength());
      auto raw_data = CanaryLib::CreateRawData(fbb, fbuffer, msg.getLength());
      fbb.Finish(raw_data);
      wrapper.add(raw_data.Union(), CanaryLib::DataType_RawData);

      CanaryLib::FlatbuffersWrapper inputWrapper;
      wrapper.Finish();
      inputWrapper.copy(wrapper.Finish());

      // validate checksum
      bool checksummed = inputWrapper.readChecksum();

      auto enc_msg = inputWrapper.getEncryptedMessage();
      uint8_t *body_buffer = (uint8_t *) enc_msg->body()->Data();

      auto content_msg = CanaryLib::GetContentMessage(body_buffer);
    }

    TEST_CASE("Reset") {
      flatbuffers::FlatBufferBuilder fbb;
      fbb.PreAlign(CanaryLib::WRAPPER_MAX_BODY_SIZE, 8);
      CanaryLib::FlatbuffersWrapper wrapper1;
      generateBaseWrapper(wrapper1);
      wrapper1.Finish(&xtea);
      validateBasic(wrapper1, &xtea);

      wrapper1.reset();
      CHECK(wrapper1.isEncryptionEnabled());
      CHECK_FALSE(wrapper1.isWriteLocked());
      CHECK_EQ(wrapper1.Size(), 0);
      CHECK_EQ(wrapper1.Types().size(), 0);
      CHECK_EQ(wrapper1.Contents().size(), 0);
    }

    TEST_CASE("Finish") {
      CanaryLib::FlatbuffersWrapper wrapper1;
      generateBaseWrapper(wrapper1);
      wrapper1.Finish();
      validateBasic(wrapper1, nullptr);

      wrapper1.reset();
      generateBaseWrapper(wrapper1);
      wrapper1.Finish(&xtea);
      validateBasic(wrapper1, &xtea);
    }

    TEST_CASE("Add") {
      CanaryLib::FlatbuffersWrapper wrapper1;
      generateBaseWrapper(wrapper1);
      CHECK_EQ(wrapper1.Types().size(), 2);
      CHECK_EQ(wrapper1.Contents().size(), 2);

      wrapper1.Finish();
      CHECK_THROWS_AS(flatbuffers::FlatBufferBuilder &fbb = wrapper1.Builder(), std::domain_error);
    }

    TEST_CASE("LoadSizeFromBuffer") {
      CanaryLib::FlatbuffersWrapper wrapper1;
      xtea.generateKey();
      uint8_t buffer[128];
      uint16_t size = 64000;
      uint16_t newSize = 3241;

      memcpy(buffer, &size, 2);
      CHECK_EQ(wrapper1.loadSizeFromBuffer(buffer), size);
      CHECK_EQ(wrapper1.Size(), size);
      memcpy(buffer, &newSize, 2);
      CHECK_EQ(wrapper1.loadSizeFromBuffer(buffer), newSize);
      CHECK_EQ(wrapper1.Size(), newSize);
    }
  }
}