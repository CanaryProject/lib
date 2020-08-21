#include "../../all.h"

namespace FlatbuffersWrapperLoadTest {
  TEST_SUITE("FlatbuffersWrapper LoadTest") {
    CanaryLib::XTEA xtea;
    std::string nameStr("myName");
    uint32_t id32 = 32412516;
    uint16_t id16 = 65000;
    uint8_t dmg = 123;

    void measureExecution(std::string name, std::function<void()> load) {
      auto started = std::chrono::high_resolution_clock::now();
      load();
      auto done = std::chrono::high_resolution_clock::now();
      spdlog::info("Process \"{}\" executed in {}ms", name, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
    }

    void loadTestNoFlatbuffer(int nMessages, int msgSize) {
      int msgLength = msgSize * 15;
      std::vector<CanaryLib::NetworkMessage> msgArray{};
      
      for (int i = 0; i < nMessages; i++) {
        CanaryLib::NetworkMessage msg;
        for (int j = 0; j < msgSize; j++) {
          msg.writeString(nameStr);
          msg.write<uint8_t>(dmg);
          msg.write<uint16_t>(id16);
          msg.write<uint32_t>(id32);
        }
        msgArray.emplace_back(msg);
      }

      for (int i = 0; i < nMessages; i++) {
        CanaryLib::NetworkMessage msg = msgArray[i];
        CanaryLib::NetworkMessage output;
        output.write(msg.getBuffer(), msg.getLength(), CanaryLib::MESSAGE_OPERATION_PEEK);
        for (int j = 0; j < msgSize; j++) {
          CHECK_EQ(output.readString(), nameStr);
          CHECK_EQ(output.read<uint8_t>(), dmg);
          CHECK_EQ(output.read<uint16_t>(), id16);
          CHECK_EQ(output.read<uint32_t>(), id32);
          CHECK_EQ(output.getLength(), msgLength);
        }
      }
    }

    void loadTestRawDataMessage(int nMessages, int msgSize) {
      CanaryLib::FlatbuffersWrapper wrapper1;
      flatbuffers::FlatBufferBuilder &fbb = wrapper1.Builder();

      for (int i = 0; i < nMessages; i++) {
        CanaryLib::NetworkMessage msg;
        for (int j = 0; j < msgSize; j++) {
          msg.writeString(nameStr);
          msg.write<uint8_t>(dmg);
          msg.write<uint16_t>(id16);
          msg.write<uint32_t>(id32);
        }
        auto buffer = fbb.CreateVector(msg.getBuffer(), msg.getLength());
        auto raw_data = CanaryLib::CreateRawData(fbb, buffer, msg.getLength());
        wrapper1.add(raw_data.Union(), CanaryLib::DataType_RawData);
      }
      wrapper1.Finish();
      
      CHECK_EQ(wrapper1.Size(), fbb.GetSize());
      CHECK(wrapper1.readChecksum());

      auto enc_msg = wrapper1.getEncryptedMessage();
      CHECK_FALSE(enc_msg->header()->encrypted());

      uint8_t *body_buffer = (uint8_t *) enc_msg->body()->Data();
      auto content_msg = CanaryLib::GetContentMessage(body_buffer);
      for (int i = 0; i < nMessages; i++) {
        CanaryLib::NetworkMessage msg;
        CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(i), CanaryLib::DataType_RawData);
        auto raw_data = content_msg->data()->GetAs<CanaryLib::RawData>(i);
        msg.write(raw_data->body()->data(), raw_data->size(), CanaryLib::MESSAGE_OPERATION_PEEK);
        CHECK_EQ(msg.getLength(), raw_data->size());
        for (int j = 0; j < msgSize; j++) {
          CHECK_EQ(msg.readString(), nameStr);
          CHECK_EQ(msg.read<uint8_t>(), dmg);
          CHECK_EQ(msg.read<uint16_t>(), id16);
          CHECK_EQ(msg.read<uint32_t>(), id32);
        }
      }
    }

    void loadTestWeaponMessage(int nMessages) {
      CanaryLib::FlatbuffersWrapper wrapper1;
      flatbuffers::FlatBufferBuilder &fbb = wrapper1.Builder();

      for (int i = 0; i < nMessages; i++) {
        auto name = fbb.CreateString(nameStr);
        auto error_data = CanaryLib::CreateErrorData(fbb, name);
        wrapper1.add(error_data.Union(), CanaryLib::DataType_ErrorData);
      }
      wrapper1.Finish();
      
      CHECK_EQ(wrapper1.Size(), fbb.GetSize());
      CHECK(wrapper1.readChecksum());

      auto enc_msg = wrapper1.getEncryptedMessage();
      CHECK_FALSE(enc_msg->header()->encrypted());

      uint8_t *body_buffer = (uint8_t *) enc_msg->body()->Data();
      auto content_msg = CanaryLib::GetContentMessage(body_buffer);
      for (int i = 0; i < nMessages; i++) {
        CHECK_EQ(content_msg->data_type()->GetEnum<CanaryLib::DataType>(i), CanaryLib::DataType_ErrorData);
        auto error = content_msg->data()->GetAs<CanaryLib::ErrorData>(1);
        CHECK_EQ(error->message()->str(), nameStr);
      }
    }

    void loadExecution(CanaryLib::DataType type, int messages, int msgSize = 1, int executions = 100) {
      auto name = std::string("(")
        .append(std::to_string(executions))
        .append("x, ").append(std::to_string(messages))
        .append(" messages with size ").append(std::to_string(msgSize))
        .append(") MultipleSmallMessages - LoadTest");

      switch (type) {
        case CanaryLib::DataType_RawData:
          measureExecution(name, [=]() { 
            for (int i = 0; i < executions; i++)
              loadTestRawDataMessage(messages, msgSize); 
          });
          return;
        case CanaryLib::DataType_ErrorData:
          measureExecution(name, [=]() { 
            for (int i = 0; i < executions; i++)
              loadTestWeaponMessage(messages); 
          });
          return;
        case CanaryLib::DataType_NONE:
        default:
          measureExecution(name, [=]() { 
            for (int i = 0; i < executions; i++)
              loadTestNoFlatbuffer(messages, msgSize); 
          });
          return;
      }
    }

    TEST_CASE("Load test, multiple small RawData") {
      loadExecution(CanaryLib::DataType_RawData, 1740);
    }

    TEST_CASE("Load test, one big message RawData") {
      loadExecution(CanaryLib::DataType_RawData, 1, 3800);
    }

    TEST_CASE("Load test, multiple small NetworkMessages") {
      loadExecution(CanaryLib::DataType_NONE, 1740);
    }

    TEST_CASE("Load test, one big NetworkMessages") {
      loadExecution(CanaryLib::DataType_NONE, 1, 3800);
    }

    TEST_CASE("Load test, multiple error data") {
      loadExecution(CanaryLib::DataType_ErrorData, 2250);
    }
  }
}