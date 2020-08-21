#include "../../all.h"

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
}
