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
  TEST_CASE("Test") {
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
}
