#include "../all.h"

template<class T>
void validateMessage(T value) {
  CanaryLib::NetworkMessage msg;
  msg.write<T>(value);
  
  CHECK(msg.getBufferPosition() == sizeof(T));
  
  msg.setBufferPosition(0);
  CHECK(msg.getLength() == sizeof(T));
  CHECK(msg.read<T>() == value);
}

TEST_SUITE("NetworkMessage") {
  TEST_CASE("NetworkMessage write/read uint8_t") {
    uint8_t value = '[';
    validateMessage<uint8_t>(value);
  }

  TEST_CASE("NetworkMessage write/read uint16_t") {
    uint16_t value = 64000;
    validateMessage<uint16_t>(value);
  }

  TEST_CASE("NetworkMessage write/read uint32_t") {
    uint32_t value = 120000;
    validateMessage<uint32_t>(value);
  }

  TEST_CASE("NetworkMessage read/write byte") {
    CanaryLib::NetworkMessage msg;
    uint8_t value = 255;
    msg.writeByte(value, CanaryLib::MESSAGE_OPERATION_PEEK);
    CHECK_EQ(msg.readByte(), value);
  }

  TEST_CASE("NetworkMessage read/write string") {
    CanaryLib::NetworkMessage msg;
    std::string value = "myString";
    msg.writeString(value, CanaryLib::MESSAGE_OPERATION_PEEK);
    CHECK_EQ(msg.readString(), value);
  }

  TEST_CASE("NetworkMessage write bytes") {
    std::string name = "myString";
    uint32_t id = 3741277123;

    CanaryLib::NetworkMessage msg;
    msg.writeString(name);
    msg.write<uint32_t>(id);
    msg.setBufferPosition(0);
    
    CanaryLib::NetworkMessage output;
    output.write(msg.getBuffer(), msg.getLength(), CanaryLib::MESSAGE_OPERATION_PEEK);
    CHECK_EQ(output.readString(), name);
    CHECK_EQ(output.read<uint32_t>(), id);
  }

  TEST_CASE("NetworkMessage write increment flag") {
    uint32_t id = 3741277123;

    CanaryLib::NetworkMessage msg;
    msg.write<uint32_t>(id);
    msg.setBufferPosition(0);
    
    CanaryLib::NetworkMessage output;
    output.write(msg.getBuffer(), msg.getLength(), CanaryLib::MESSAGE_OPERATION_PEEK);
    CHECK_EQ(output.getBufferPosition(), 0);
    CHECK_EQ(output.getLength(), sizeof(uint32_t));
    output.reset();
    output.write(msg.getBuffer(), msg.getLength(), CanaryLib::MESSAGE_OPERATION_STANDARD);
    CHECK_EQ(output.getBufferPosition(), sizeof(uint32_t));
    CHECK_EQ(output.getLength(), sizeof(uint32_t));
  }

  TEST_CASE("NetworkMessage writePaddingBytes") {
    CanaryLib::NetworkMessage msg;
    uint8_t n = 8;
    for (uint8_t i = 0; i <= n; i++) {
      msg.reset();
      msg.writePaddingBytes(i);
      CHECK_EQ(msg.getLength(), i);
    }
    CHECK_EQ(msg.getBufferPosition(), n);
  }

  TEST_CASE("NetworkMessage skip") {
    CanaryLib::NetworkMessage msg;
    uint8_t n = 8;
    msg.skip(n);
    CHECK_EQ(msg.getBufferPosition(), n);
  }

  TEST_CASE("NetworkMessage skip<T>") {
    CanaryLib::NetworkMessage msg;
    msg.skip<double>();
    CHECK_EQ(msg.getBufferPosition(), sizeof(double));
    msg.reset();
    msg.skip<uint8_t>();
    CHECK_EQ(msg.getBufferPosition(), sizeof(uint8_t));
    msg.reset();
    msg.skip<uint16_t>();
    CHECK_EQ(msg.getBufferPosition(), sizeof(uint16_t));
    msg.reset();
    msg.skip<uint32_t>();
    CHECK_EQ(msg.getBufferPosition(), sizeof(uint32_t));
    msg.reset();
  }

  TEST_CASE("Flatbuffer input/output test") {
    CanaryLib::XTEA xtea;
    std::vector<flatbuffers::Offset<void>> contents;
    std::vector<uint8_t> types;

    std::string nameStr = "myNamgasfaasde";
    uint32_t id = 3294967295;
    flatbuffers::FlatBufferBuilder fbb, fbb2, fbb3;
    fbb.PreAlign(65000, 8);

    auto name = fbb.CreateString(nameStr);
    auto x = CanaryLib::CreatePlayerData(fbb, id, name);
    fbb.Finish(x);
    contents.emplace_back(x.Union());
    types.emplace_back(CanaryLib::DataType_PlayerData);

    auto name2 = fbb.CreateString("nameStr");
    auto y = CanaryLib::CreateWeaponData(fbb, id, name2, 100).Union();
    fbb.Finish(y);
    contents.emplace_back(y.Union());
    types.emplace_back(CanaryLib::DataType_WeaponData);

    auto types_vec = fbb.CreateVector(types);
    auto contents_vec = fbb.CreateVector(contents);

    auto content_msg = CanaryLib::CreateContentMessage(fbb, types_vec, contents_vec);
    fbb.Finish(content_msg);

    uint16_t size = fbb.GetSize();
    uint8_t *body_buffer = fbb.GetBufferPointer();
    xtea.encrypt(size, body_buffer);

    auto body = fbb.CreateVector(body_buffer, size);
    uint32_t checksum = CanaryLib::FlatbuffersWrapper::getChecksum(body_buffer, size);

    auto encrypted_message = CanaryLib::CreateEncryptedMessage(fbb, checksum, size, false, body);

    fbb.Finish(encrypted_message);  

    auto enc_msg = CanaryLib::GetEncryptedMessage(fbb.GetBufferPointer());

    // auto final = CanaryLib::GetEncryptedMessage(fbb.GetBufferPointer());
    CHECK_EQ(enc_msg->message_size(), size);
    CHECK_FALSE(enc_msg->encrypted());

    auto finalbuf = enc_msg->body()->Data();

    checksum = CanaryLib::FlatbuffersWrapper::getChecksum(finalbuf, size);

    CHECK_EQ(checksum, enc_msg->checksum());

    xtea.decrypt(size, (uint8_t *) finalbuf);
    auto content = CanaryLib::GetContentMessage(finalbuf);
    CHECK_EQ(content->data_type()->GetEnum<CanaryLib::DataType>(0), CanaryLib::DataType_PlayerData );

    CHECK_EQ(reinterpret_cast<const CanaryLib::PlayerData *>(content->data()->Get(0))->name()->str(), nameStr);
    CHECK_EQ(content->data()->GetAs<CanaryLib::WeaponData>(1)->damage(), 100);
  }
}
