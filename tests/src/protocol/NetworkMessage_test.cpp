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
    flatbuffers::FlatBufferBuilder fbb;
    auto header = CanaryLib::CreateHeader(fbb, 1010, 101);
    auto player = CanaryLib::CreatePlayerData(fbb, 3294967295, fbb.CreateString("LucasGoulart"));
    auto message = CanaryLib::CreateMessage(fbb, header, CanaryLib::Data_PlayerData, player.Union());
    fbb.Finish(message);
    uint8_t *buf = fbb.GetBufferPointer();
    int size = fbb.GetSize();
    auto readMessage = CanaryLib::GetMessage(buf);

    std::string name = std::string(readMessage->data_as_PlayerData()->name()->c_str());
    std::cout << name << std::endl;
    std::cout << size << std::endl;
  }
}
