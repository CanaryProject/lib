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

TEST_SUITE("NetworkMessage Test") {
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

    auto header = CanaryLib::CreateHeader(fbb, size, size);
    auto encrypted_bytes = fbb.CreateVector(buf, size);

    auto encrypted_message = CanaryLib::CreateEncryptedMessage(fbb, header, encrypted_bytes);
    fbb.Finish(encrypted_message);

    auto final = CanaryLib::GetEncryptedMessage(fbb.GetBufferPointer());
    CHECK_EQ(final->header()->checksum(), size);
    CHECK_EQ(final->header()->encrypted_size(), size);

    const uint8_t* final_data = final->data()->data();
    auto final_msg = CanaryLib::GetMessage(final_data);
    CHECK(!!final_msg->data());
    CHECK_EQ(final_msg->data_type(), CanaryLib::Data_PlayerData);
    CHECK_EQ(final_msg->data_as_PlayerData()->name()->c_str(), nameStr);
    CHECK_EQ(final_msg->data_as_PlayerData()->id(), id);
  }
  TEST_CASE("Write>Encode>Decode>Read messages with flatbuffers") {
    // Const variables for testing purpose
    std::string name = "Mr. Someone";
    uint32_t id = 3294967295;
    Position pos{63201, 5513, 8};
    
    // Create the default message that will be our buffer
    CanaryLib::NetworkMessage msg;
    msg.write<uint32_t>(id);
    msg.writeString(name);
    msg.write<Position>(pos);

    // Validate xtea size and write padding
    uint16_t msg_size = msg.getLength();
    if ((msg_size % 8) != 0) {
      msg.writePaddingBytes(8 - (msg_size % 8));
    }
    msg_size = msg.getLength();
    msg.setBufferPosition(CanaryLib::MAX_HEADER_SIZE);
    
    // Encrypt input
    CanaryLib::XTEA().encrypt(msg_size, msg.getOutputBuffer());

    // Start flabuffer creation
    flatbuffers::FlatBufferBuilder fbb;
    auto header = CanaryLib::CreateHeader(fbb, msg_size, msg_size);
    // create flatbuffer vector with the outputbuffer
    auto encrypted_bytes = fbb.CreateVector(msg.getOutputBuffer(), msg_size);

    auto encrypted_message = CanaryLib::CreateEncryptedMessage(fbb, header, encrypted_bytes);
    fbb.Finish(encrypted_message);

    auto final = CanaryLib::GetEncryptedMessage(fbb.GetBufferPointer());
    auto encrypted_size = final->header()->encrypted_size();

    // Validade header
    CHECK_EQ(final->header()->checksum(), msg_size);
    CHECK_EQ(encrypted_size, msg_size);

    // Validate Size
    CHECK_EQ(final->data()->size(), msg_size);

    // Read message from flatbuffer
    CanaryLib::NetworkMessage output;
    output.write(final->data()->data(), msg_size, CanaryLib::MESSAGE_INCREMENT_SIZE);

    // Validade buffer position (must be initial pos)
    CHECK_EQ(output.getBufferPosition(), CanaryLib::MAX_HEADER_SIZE);

    // Validade decrypted message values
    CanaryLib::XTEA().decrypt(msg_size, output.getOutputBuffer());
    CHECK_EQ(output.read<uint32_t>(), id);
    CHECK_EQ(output.readString(), name);
    CHECK_EQ(output.read<Position>(), pos);
  }
}
