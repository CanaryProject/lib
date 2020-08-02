// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HEADER_CANARYLIB_H_
#define FLATBUFFERS_GENERATED_HEADER_CANARYLIB_H_

#include "flatbuffers/flatbuffers.h"

namespace CanaryLib {

struct Header;
struct HeaderBuilder;

struct Header FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef HeaderBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CHECKSUM = 4,
    VT_ENCRYPTED_SIZE = 6,
    VT_MESSAGE_SIZE = 8,
    VT_ENCRYPTED = 10
  };
  uint32_t checksum() const {
    return GetField<uint32_t>(VT_CHECKSUM, 0);
  }
  uint16_t encrypted_size() const {
    return GetField<uint16_t>(VT_ENCRYPTED_SIZE, 0);
  }
  uint16_t message_size() const {
    return GetField<uint16_t>(VT_MESSAGE_SIZE, 0);
  }
  bool encrypted() const {
    return GetField<uint8_t>(VT_ENCRYPTED, 1) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_CHECKSUM) &&
           VerifyField<uint16_t>(verifier, VT_ENCRYPTED_SIZE) &&
           VerifyField<uint16_t>(verifier, VT_MESSAGE_SIZE) &&
           VerifyField<uint8_t>(verifier, VT_ENCRYPTED) &&
           verifier.EndTable();
  }
};

struct HeaderBuilder {
  typedef Header Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_checksum(uint32_t checksum) {
    fbb_.AddElement<uint32_t>(Header::VT_CHECKSUM, checksum, 0);
  }
  void add_encrypted_size(uint16_t encrypted_size) {
    fbb_.AddElement<uint16_t>(Header::VT_ENCRYPTED_SIZE, encrypted_size, 0);
  }
  void add_message_size(uint16_t message_size) {
    fbb_.AddElement<uint16_t>(Header::VT_MESSAGE_SIZE, message_size, 0);
  }
  void add_encrypted(bool encrypted) {
    fbb_.AddElement<uint8_t>(Header::VT_ENCRYPTED, static_cast<uint8_t>(encrypted), 1);
  }
  explicit HeaderBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Header> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Header>(end);
    return o;
  }
};

inline flatbuffers::Offset<Header> CreateHeader(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t checksum = 0,
    uint16_t encrypted_size = 0,
    uint16_t message_size = 0,
    bool encrypted = true) {
  HeaderBuilder builder_(_fbb);
  builder_.add_checksum(checksum);
  builder_.add_message_size(message_size);
  builder_.add_encrypted_size(encrypted_size);
  builder_.add_encrypted(encrypted);
  return builder_.Finish();
}

inline const CanaryLib::Header *GetHeader(const void *buf) {
  return flatbuffers::GetRoot<CanaryLib::Header>(buf);
}

inline const CanaryLib::Header *GetSizePrefixedHeader(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<CanaryLib::Header>(buf);
}

inline bool VerifyHeaderBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<CanaryLib::Header>(nullptr);
}

inline bool VerifySizePrefixedHeaderBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<CanaryLib::Header>(nullptr);
}

inline void FinishHeaderBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::Header> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedHeaderBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::Header> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace CanaryLib

#endif  // FLATBUFFERS_GENERATED_HEADER_CANARYLIB_H_
