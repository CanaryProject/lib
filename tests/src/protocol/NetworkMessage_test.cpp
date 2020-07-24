#include "../all.h"

template<class T>
void validateMessage(T value) {
  CanaryLib::NetworkMessage msg;
  msg.write<T>(value);
  
  CHECK(msg.getBufferPosition() == (CanaryLib::MAX_HEADER_SIZE + sizeof(T)));
  
  msg.setBufferPosition(CanaryLib::MAX_HEADER_SIZE);
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
    msg.writeByte(value);
    msg.setBufferPosition(CanaryLib::MAX_HEADER_SIZE);
    CHECK_EQ(msg.readByte(), value);
  }
  TEST_CASE("NetworkMessage read/write string") {
    CanaryLib::NetworkMessage msg;
    std::string value = "myString";
    msg.writeString(value);
    msg.setBufferPosition(CanaryLib::MAX_HEADER_SIZE);
    CHECK_EQ(msg.readString(), value);
  }
  TEST_CASE("NetworkMessage write bytes") {
    std::string name = "myString";
    uint32_t id = 3741277123;

    CanaryLib::NetworkMessage msg;
    msg.writeString(name);
    msg.write<uint32_t>(id);
    msg.setBufferPosition(CanaryLib::MAX_HEADER_SIZE);
    
    CanaryLib::NetworkMessage output;
    output.write(msg.getOutputBuffer(), msg.getLength(), CanaryLib::MESSAGE_INCREMENT_SIZE);
    CHECK_EQ(output.readString(), name);
    CHECK_EQ(output.read<uint32_t>(), id);
  }
  TEST_CASE("NetworkMessage write increment flag") {
    uint32_t id = 3741277123;

    CanaryLib::NetworkMessage msg;
    msg.write<uint32_t>(id);
    msg.setBufferPosition(CanaryLib::MAX_HEADER_SIZE);
    
    CanaryLib::NetworkMessage output;
    output.write(msg.getOutputBuffer(), msg.getLength(), CanaryLib::MESSAGE_INCREMENT_SIZE);
    CHECK_EQ(output.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE);
    CHECK_EQ(output.getLength(), sizeof(uint32_t));
    output.reset();
    output.write(msg.getOutputBuffer(), msg.getLength(), CanaryLib::MESSAGE_INCREMENT_BUFFER);
    CHECK_EQ(output.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + sizeof(uint32_t));
    CHECK_EQ(output.getLength(), 0);
    output.reset();
    output.write(msg.getOutputBuffer(), msg.getLength(), CanaryLib::MESSAGE_INCREMENT_BUFFER_AND_SIZE);
    CHECK_EQ(output.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + sizeof(uint32_t));
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
    CHECK_EQ(msg.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE);
  }
  TEST_CASE("NetworkMessage skip") {
    CanaryLib::NetworkMessage msg;
    uint8_t n = 8;
    msg.skip(n);
    CHECK_EQ(msg.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + n);
  }
  TEST_CASE("NetworkMessage skip<T>") {
    CanaryLib::NetworkMessage msg;
    msg.skip<double>();
    CHECK_EQ(msg.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + sizeof(double));
    msg.reset();
    msg.skip<uint8_t>();
    CHECK_EQ(msg.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + sizeof(uint8_t));
    msg.reset();
    msg.skip<uint16_t>();
    CHECK_EQ(msg.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + sizeof(uint16_t));
    msg.reset();
    msg.skip<uint32_t>();
    CHECK_EQ(msg.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE + sizeof(uint32_t));
    msg.reset();
  }
  TEST_CASE("Flatbuffer input/output test") {
    std::string nameStr = "myName";
    uint32_t id = 3294967295;
    flatbuffers::FlatBufferBuilder fbb;
    auto name = fbb.CreateString(nameStr);
    auto player = CanaryLib::CreatePlayerData(fbb, id, name);
    fbb.Finish(player);

    uint8_t *buf = fbb.GetBufferPointer();
    auto message = CanaryLib::CreateMessage(fbb, CanaryLib::Data_PlayerData, player.Union());
    fbb.ForceVectorAlignment(fbb.GetSize(), sizeof(uint8_t), 8);

    fbb.Finish(message);
    int size = fbb.GetSize();

    CHECK_EQ(size % 8, 0);
    
    buf = fbb.GetBufferPointer();

    auto header = CanaryLib::CreateHeader(fbb, size, size, size);
    auto encrypted_bytes = fbb.CreateVector(buf, size);

    auto encrypted_message = CanaryLib::CreateEncryptedMessage(fbb, header, encrypted_bytes);
    fbb.Finish(encrypted_message);

    auto final = CanaryLib::GetEncryptedMessage(fbb.GetBufferPointer());
    CHECK_EQ(final->header()->checksum(), size);
    CHECK_EQ(final->header()->encrypted_size(), size);
    CHECK_EQ(final->header()->size(), size);

    const uint8_t* final_data = final->data()->data();
    auto final_msg = CanaryLib::GetMessage(final_data);
    CHECK(!!final_msg->data());
    CHECK_EQ(final_msg->data_type(), CanaryLib::Data_PlayerData);
    CHECK_EQ(final_msg->data_as_PlayerData()->name()->c_str(), nameStr);
    CHECK_EQ(final_msg->data_as_PlayerData()->id(), id);
  }
}
