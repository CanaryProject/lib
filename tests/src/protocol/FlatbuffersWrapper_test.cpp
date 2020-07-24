#include "../all.h"

TEST_SUITE("FlatbuffersWrapper") {
  TEST_CASE("Initialize wrapper with msg") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::NetworkMessage msg;
    msg.write(buffer, randomMsg.size(), CanaryLib::MESSAGE_INCREMENT_SIZE);

    CanaryLib::FlatbuffersWrapper wrapper(msg);

    CHECK_EQ(wrapper.contentSize(), msg.getLength());
    CHECK_EQ(std::string((char *) wrapper.content()), randomMsg);
  }

  TEST_CASE("Initialize wrapper with buffer and size") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper(buffer, randomMsg.size());

    CHECK_EQ(wrapper.contentSize(), randomMsg.size());
    CHECK_EQ(std::string((char *) wrapper.content()), randomMsg);
  }

  TEST_CASE("Copy wrappers") {
    CanaryLib::FlatbuffersWrapper wrapper1;
    CanaryLib::FlatbuffersWrapper wrapper2;

    wrapper1.buildFlatbuffers();
    wrapper2.copy(wrapper1.buffer());

    CHECK_EQ(wrapper1.size(), wrapper2.size());
  }

  TEST_CASE("BuildFlatbuffers") {
    CanaryLib::FlatbuffersWrapper wrapper1;

    wrapper1.buildFlatbuffers();
    auto encryptedMsg = wrapper1.loadFlatbuffers();
    auto header = encryptedMsg->header();
    CHECK_EQ(header->size(), 0);
    CHECK_EQ(header->checksum(), CanaryLib::NetworkMessage::getChecksum(wrapper1.content(), wrapper1.contentSize()));
    CHECK_EQ(header->encrypted_size(), 0);
    CHECK_EQ(header->format(), CanaryLib::ContentFormat_RawMessage);
  }

  TEST_CASE("BuildFlatbuffers with format") {
    CanaryLib::FlatbuffersWrapper wrapper1;

    wrapper1.buildFlatbuffers(CanaryLib::ContentFormat_Flatbuffers);
    auto encryptedMsg = wrapper1.loadFlatbuffers();
    auto header = encryptedMsg->header();
    CHECK_EQ(header->size(), 0);
    CHECK_EQ(header->checksum(), CanaryLib::NetworkMessage::getChecksum(wrapper1.content(), wrapper1.contentSize()));
    CHECK_EQ(header->encrypted_size(), 0);
    CHECK_EQ(header->format(), CanaryLib::ContentFormat_Flatbuffers);
  }

  TEST_CASE("LoadFlatbuffers") {
    CanaryLib::FlatbuffersWrapper wrapper1;

    wrapper1.buildFlatbuffers(CanaryLib::ContentFormat_Flatbuffers);
    auto encryptedMsg = wrapper1.loadFlatbuffers();

    CHECK_EQ(encryptedMsg->header()->encrypted_size(), wrapper1.contentSize());
  }

  TEST_CASE("ToRawMessage") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::NetworkMessage msg;
    msg.write(buffer, randomMsg.size(), CanaryLib::MESSAGE_INCREMENT_SIZE);

    CanaryLib::FlatbuffersWrapper wrapper(msg);

    CanaryLib::NetworkMessage output;
    wrapper.toRawMessage(output);

    CHECK_EQ(output.getLength(), msg.getLength());
    CHECK_EQ(output.readString(output.getLength()), msg.readString(msg.getLength()));
  }

  TEST_CASE("Encrypt/Decrypt XTEA") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper(buffer, randomMsg.size());

    wrapper.encryptXTEA();
    CHECK_EQ(wrapper.contentSize() % 8, 0);
    CHECK_NE(std::string((char *) wrapper.content()), randomMsg);
    wrapper.decryptXTEA();

    uint8_t padding = (8 - randomMsg.size() % 8);

    char output[512];
    memcpy(output, wrapper.content(), wrapper.contentSize() - padding);

    CHECK_EQ(wrapper.contentSize(), randomMsg.size() + padding);
    CHECK_EQ(std::string(output), randomMsg);
  }

  TEST_CASE("PrepareXTEAEncryption") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper(buffer, randomMsg.size());
    uint16_t size = wrapper.prepareXTEAEncryption();

    CHECK_EQ(size, randomMsg.size() + (8 - randomMsg.size() % 8));
  }

  TEST_CASE("Write") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.write(buffer, randomMsg.size());

    CHECK_EQ(wrapper.size(), randomMsg.size());
    CHECK_EQ(std::string((char *) wrapper.buffer() + CanaryLib::WRAPPER_HEADER_SIZE), randomMsg);
  }

  TEST_CASE("WriteSize") {
    uint8_t buffer[512] = "Random buf";
    std::string randomMsg = std::string((char *) buffer);

    CanaryLib::FlatbuffersWrapper wrapper;
    wrapper.writeSize(randomMsg.size());

    CHECK_EQ(wrapper.size(), randomMsg.size());
  }

  TEST_CASE("Flatbuffer wrap message") {
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
    CanaryLib::FlatbuffersWrapper wrapper(msg);
    wrapper.encryptXTEA();
    wrapper.buildFlatbuffers();

    // validation
    uint32_t checksum = CanaryLib::NetworkMessage::getChecksum(wrapper.content(), wrapper.contentSize());

    // Validade header
    auto final = CanaryLib::GetEncryptedMessage(wrapper.body());
    CHECK_EQ(final->header()->checksum(), checksum);
    CHECK_EQ(final->header()->size(), wrapper.contentSize());
    CHECK_EQ(final->header()->encrypted_size(), wrapper.encryptedSize());
    CHECK_EQ(final->header()->format(), CanaryLib::ContentFormat_RawMessage);

    // Validate Size
    CHECK_EQ(final->data()->size(), wrapper.contentSize());

    // Receive message from flatbuffers wrapper
    CanaryLib::FlatbuffersWrapper outWrapper(wrapper.buffer());
    CanaryLib::NetworkMessage output;
    outWrapper.toRawMessage(output);

    // Validate checksum before decrypt
    uint32_t recvChecksum = CanaryLib::NetworkMessage::getChecksum(output.getOutputBuffer(), output.getLength());
    CHECK_EQ(recvChecksum, final->header()->checksum());

    // Decrypt
    outWrapper.decryptXTEA();
    outWrapper.toRawMessage(output);

    // Validade decrypted message values
    CHECK_EQ(output.read<uint32_t>(), id);
    CHECK_EQ(output.readString(), name);
    CHECK_EQ(output.read<Position>().x, pos.x);
  }

  TEST_CASE("Flatbuffer wrap random buffer test") {
    uint8_t buffer[512];
    std::string randomMsg = "This is a random msg.";
    uint16_t size = randomMsg.size();
    memcpy(buffer, randomMsg.c_str(), size);

    // wrapping
    CanaryLib::FlatbuffersWrapper wrapper(buffer, size);
    wrapper.encryptXTEA();
    wrapper.buildFlatbuffers(CanaryLib::ContentFormat_Flatbuffers);

    // validation
    uint32_t checksum = CanaryLib::NetworkMessage::getChecksum(wrapper.content(), wrapper.contentSize());

    // Validade header
    auto final = CanaryLib::GetEncryptedMessage(wrapper.body());
    CHECK_EQ(final->header()->checksum(), checksum);
    CHECK_EQ(final->header()->size(), wrapper.contentSize());
    CHECK_EQ(final->header()->encrypted_size(), wrapper.encryptedSize());
    CHECK_EQ(final->header()->format(), CanaryLib::ContentFormat_Flatbuffers);

    // Validate Size
    CHECK_EQ(final->data()->size(), wrapper.contentSize());

    // Receive message from flatbuffers wrapper
    CanaryLib::FlatbuffersWrapper outWrapper(wrapper.buffer());
    CanaryLib::NetworkMessage output;
    outWrapper.toRawMessage(output);

    // Validate checksum before decrypt
    uint32_t recvChecksum = CanaryLib::NetworkMessage::getChecksum(outWrapper.content(), outWrapper.contentSize());
    CHECK_EQ(recvChecksum, final->header()->checksum());

    // Decrypt
    outWrapper.decryptXTEA();
    char outputMsg[512];

    uint8_t padding = (8 - randomMsg.size() % 8);

    memcpy(outputMsg, outWrapper.content(), outWrapper.contentSize() - padding);
    CHECK_EQ(outWrapper.contentSize(), randomMsg.size() + padding);
    CHECK_EQ(std::string(outputMsg), randomMsg);
  }
}