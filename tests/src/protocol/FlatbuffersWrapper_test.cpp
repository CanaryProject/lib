#include "../all.h"

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
    auto player_data = CanaryLib::CreatePlayerData(fbb, id32, name);
    fbb.Finish(player_data);
    wrapper1.add(player_data.Union(), CanaryLib::DataType_PlayerData);

    auto name2 = fbb.CreateString(nameStr);
    auto weapon_data = CanaryLib::CreateWeaponData(fbb, id16, name2, dmg);
    fbb.Finish(weapon_data);
    wrapper1.add(weapon_data.Union(), CanaryLib::DataType_WeaponData);

    auto buffer = fbb.CreateVector((uint8_t *) rawStr.c_str(), rawStr.size());
    auto raw_data = CanaryLib::CreateRawData(fbb, buffer, rawStr.size());
    fbb.Finish(raw_data);
    wrapper1.add(raw_data.Union(), CanaryLib::DataType_RawData);
  }

  void validateBasic(CanaryLib::FlatbuffersWrapper &wrapper1, CanaryLib::XTEA* _xtea = nullptr) {
    CHECK(wrapper1.readChecksum());
    CHECK_EQ(wrapper1.Encrypted(), !!_xtea);

    auto enc_msg = wrapper1.getEncryptedMessage();
    uint8_t *body_buffer = (uint8_t *) enc_msg->body()->Data();

    if (_xtea)
      _xtea->decrypt(enc_msg->header()->message_size(), body_buffer);

    auto content_msg = CanaryLib::GetContentMessage(body_buffer);

    CHECK_EQ(enc_msg->header()->encrypted(), !!_xtea);
    CHECK_EQ(content_msg->data()->size(), 3);

    CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(0), CanaryLib::DataType_PlayerData);
    CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(1), CanaryLib::DataType_WeaponData);
    CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(2), CanaryLib::DataType_RawData);

    auto player = content_msg->data()->GetAs<CanaryLib::PlayerData>(0);
    CHECK_EQ(player->name()->str(), nameStr);
    CHECK_EQ(player->id(), id32);

    auto weapon = content_msg->data()->GetAs<CanaryLib::WeaponData>(1);
    CHECK_EQ(weapon->name()->str(), nameStr);
    CHECK_EQ(weapon->id(), id16);
    CHECK_EQ(weapon->damage(), dmg);

    auto raw = content_msg->data()->GetAs<CanaryLib::RawData>(2);
    
    CHECK_EQ(std::string((char *) raw->body()->data()), rawStr);
    CHECK_EQ(raw->size(), rawStr.size());
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
    CHECK(wrapper1.EncryptionEnabled());
    CHECK_FALSE(wrapper1.Encrypted());
    CHECK_FALSE(!!wrapper1.getEncryptedMessage());
    CHECK_EQ(wrapper1.Size(), fbb.GetSize());
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
    CHECK_EQ(wrapper1.Types().size(), 3);
    CHECK_EQ(wrapper1.Contents().size(), 3);

    wrapper1.Finish();

    flatbuffers::FlatBufferBuilder &fbb = wrapper1.Builder();
    auto buffer = fbb.CreateVector((uint8_t *) rawStr.c_str(), rawStr.size());
    auto raw_data = CanaryLib::CreateRawData(fbb, buffer);
    fbb.Finish(raw_data);
    CHECK_FALSE(wrapper1.add(raw_data.Union(), CanaryLib::DataType_RawData));
  }

  TEST_CASE("LoadSizeFromBuffer") {
    xtea.generateKey();
    uint8_t buffer[128];
    uint16_t size = 64000;
    uint16_t newSize = 3241;

    memcpy(buffer, &size, 2);
    CHECK_EQ(CanaryLib::FlatbuffersWrapper::loadSizeFromBuffer(buffer), size);
    memcpy(buffer, &newSize, 2);
    CHECK_EQ(CanaryLib::FlatbuffersWrapper::loadSizeFromBuffer(buffer), newSize);
  }
}