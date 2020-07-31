#include "../all.h"

TEST_SUITE("Crypt Test") {
  TEST_CASE("XTEA encrypt/decrypt") {
    uint8_t buffer[8];
    std::string str = "myString";
    memcpy(buffer, (void *) str.c_str(), str.length());
    CanaryLib::XTEA xtea;
    xtea.encrypt(str.size(), buffer);
    CHECK_NE(std::string((char*) buffer), str);
    xtea.decrypt(str.size(), buffer);
    CHECK_EQ(std::string((char*) buffer), str);
  }
}
