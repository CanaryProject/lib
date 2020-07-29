#include "../all.h"

TEST_SUITE("FlatbuffersWrapper") {
  TEST_CASE("Copy wrappers - serialized defaults to true") {
    CanaryLib::FlatbuffersWrapper wrapper1;
    CanaryLib::FlatbuffersWrapper wrapper2;

    wrapper1.serialize();
    wrapper2.copy(wrapper1.buffer());

    CHECK_EQ(wrapper1.size(), wrapper2.size());
    CHECK(wrapper2.isSerialized());
  }

  TEST_CASE("loadBufferSize") {
    uint8_t buffer[128];
    uint16_t size = 64000;
    uint16_t newSize = 3241;
    CanaryLib::FlatbuffersWrapper wrapper1;

    memcpy(buffer, &size, 2);
    wrapper1.loadBufferSize(buffer);
    CHECK_EQ(wrapper1.size(), size);
    memcpy(buffer, &newSize, 2);
    wrapper1.loadBufferSize(buffer);
    CHECK_EQ(wrapper1.size(), newSize);
  }

  TEST_CASE("Serialize") {
    CanaryLib::FlatbuffersWrapper wrapper1;

    uint32_t checksum = wrapper1.checksum();
    wrapper1.serialize();

    CHECK(wrapper1.isSerialized());
    
    auto encryptedMsg = wrapper1.buildEncryptedMessage();
    auto header = encryptedMsg->header();
    CHECK_EQ(header->encrypted_size(), 0);
    CHECK_EQ(header->checksum(), checksum);

    CHECK_THROWS_AS(wrapper1.serialize(), std::domain_error);
  }

  TEST_CASE("Deserialize") {
    CanaryLib::FlatbuffersWrapper wrapper1;
    std::string str = std::string("myMsg");
    wrapper1.write(str.c_str(), str.size());
    wrapper1.serialize();
    wrapper1.deserialize();

    CHECK_FALSE(wrapper1.isSerialized());
    CHECK_EQ(std::string((char *) wrapper1.body()), str);
    CHECK_EQ(wrapper1.size(), str.size());

    CHECK_THROWS_AS(wrapper1.deserialize(), std::domain_error);
  }

  TEST_CASE("buildEncryptedMessage") {
    CanaryLib::FlatbuffersWrapper wrapper1;
    std::string str = std::string("myMsg");
    wrapper1.write(str.c_str(), str.size());

    wrapper1.serialize();
    auto encryptedMsg = wrapper1.buildEncryptedMessage();

    CHECK(wrapper1.readChecksum());
    CHECK_EQ(encryptedMsg->header()->encrypted_size(), str.size());
    CHECK_EQ(encryptedMsg->header()->message_size(), str.size());
    CHECK_EQ(std::string((char *) encryptedMsg->body()->data()), str);
  }

  TEST_CASE("buildRawMessage") {
    std::string randomMsg = std::string("myMsg");

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(randomMsg.c_str(), randomMsg.size());

    CanaryLib::NetworkMessage output = wrapper.buildRawMessage();

    CHECK_EQ(output.getLength(), randomMsg.size());
    CHECK_EQ(std::string((char *) output.getDataBuffer()).c_str(), randomMsg);
  }

  TEST_CASE("Encrypt/Decrypt XTEA") {
    CanaryLib::XTEA xtea;
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(buffer, randomMsg.size());

    wrapper.encryptXTEA(xtea);
    CHECK_EQ(wrapper.size() % 8, 0);
    CHECK_NE(std::string((char *) wrapper.body()), randomMsg);
    wrapper.decryptXTEA(xtea);

    char output[512];
    memcpy(output, wrapper.body(), wrapper.size());

    CHECK_EQ(wrapper.size(), randomMsg.size());
    CHECK_EQ(std::string(output), randomMsg);
  }

  TEST_CASE("PrepareXTEAEncryption") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(buffer, randomMsg.size());
    uint16_t size = wrapper.prepareXTEAEncryption();

    CHECK_EQ(size, randomMsg.size() + (8 - randomMsg.size() % 8));
  }

  TEST_CASE("Write") {
    std::string randomMsg = std::string("myMsg");

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(randomMsg.c_str(), randomMsg.size());

    CHECK_EQ(wrapper.size(), randomMsg.size());
    CHECK_EQ(std::string((char *) wrapper.body()), randomMsg);
  }

  TEST_CASE("Write - append") {
    uint8_t buffer[4] = { 1, 2, 3, 4 };

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(buffer, 4);
    wrapper.write(buffer, 4, true);

    // shouldn't write serialized
    wrapper.serialize();
    wrapper.write(buffer, 4, true);
    wrapper.deserialize();
    
    uint8_t buffer2[8] = { 1, 2, 3, 4, 1, 2, 3, 4 };
    
    CHECK_EQ(wrapper.size(), 8);
    for (uint8_t i = 0; i < wrapper.size(); i++) {
      CHECK_EQ(wrapper.body()[i], buffer2[i]);
    }
  }

  TEST_CASE("WriteSize") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.writeSize(randomMsg.size());

    CHECK_EQ(wrapper.size(), randomMsg.size());
  }

  TEST_CASE("Flatbuffer wrap message") {
    CanaryLib::XTEA xtea;
    struct Position {
      uint16_t x = 0;
    };

    // Const variables for testing purpose
    std::string name = "Mr. Someone";
    uint32_t id = 3294967295;
    Position pos{63201};

    // Create the default message that will be our buffer
    CanaryLib::NetworkMessage msg;
    msg.write<uint32_t>(id);
    msg.writeString(name);
    msg.write<Position>(pos);

    // wrapping
    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(msg.getDataBuffer(), msg.getLength());
    wrapper.encryptXTEA(xtea);

    uint16_t msgSize = wrapper.msgSize();

    wrapper.serialize();

    // Validade header
    auto enc_msg = wrapper.buildEncryptedMessage();
    CHECK(wrapper.readChecksum());
    CHECK_EQ(enc_msg->header()->encrypted_size(), enc_msg->body()->size());
    CHECK_EQ(enc_msg->header()->message_size(), msgSize);

    // Receive message from flatbuffers wrapper
    CanaryLib::FlatbuffersWrapper outWrapper;
    outWrapper.copy(wrapper.buffer(), true);

    // Decrypt
    outWrapper.deserialize();
    outWrapper.decryptXTEA(xtea);
    CanaryLib::NetworkMessage output = outWrapper.buildRawMessage();

    // Validade decrypted message values
    CHECK_EQ(output.read<uint32_t>(), id);
    CHECK_EQ(output.readString(), name);
    CHECK_EQ(output.read<Position>().x, pos.x);
  }

  TEST_CASE("Flatbuffer wrap random buffer test") {
    CanaryLib::XTEA xtea;
    uint8_t buffer[512];
    std::string randomMsg = "This is a random msg.";
    uint16_t size = randomMsg.size();
    memcpy(buffer, randomMsg.c_str(), size);

    // wrapping
    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(buffer, size);
    wrapper.encryptXTEA(xtea);

    uint16_t msgSize = wrapper.msgSize();

    wrapper.serialize();

    // Validade header
    auto enc_msg = CanaryLib::GetEncryptedMessage(wrapper.body());
    CHECK(wrapper.readChecksum());
    CHECK_EQ(enc_msg->header()->encrypted_size(), enc_msg->body()->size());
    CHECK_EQ(enc_msg->header()->message_size(), msgSize);

    // Receive message from flatbuffers wrapper
    CanaryLib::FlatbuffersWrapper outWrapper;
    outWrapper.copy(wrapper.buffer(), true);
    outWrapper.deserialize();
    CanaryLib::NetworkMessage output = outWrapper.buildRawMessage();

    // Decrypt
    outWrapper.decryptXTEA(xtea);
    char outputMsg[512];

    memcpy(outputMsg, outWrapper.body(), outWrapper.size());
    CHECK_EQ(outWrapper.size(), randomMsg.size());
    CHECK_EQ(std::string(outputMsg), randomMsg);
  }
}