// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CONTENTMESSAGE_CANARYLIB_H_
#define FLATBUFFERS_GENERATED_CONTENTMESSAGE_CANARYLIB_H_

#include "flatbuffers/flatbuffers.h"

#include "characters_list_data_generated.h"
#include "error_data_generated.h"
#include "login_data_generated.h"
#include "player_data_generated.h"
#include "raw_data_generated.h"
#include "weapon_data_generated.h"

namespace CanaryLib {

struct ContentMessage;
struct ContentMessageBuilder;

enum DataType {
  DataType_NONE = 0,
  DataType_CharactersListData = 1,
  DataType_ErrorData = 2,
  DataType_LoginData = 3,
  DataType_RawData = 4,
  DataType_PlayerData = 5,
  DataType_WeaponData = 6,
  DataType_MIN = DataType_NONE,
  DataType_MAX = DataType_WeaponData
};

inline const DataType (&EnumValuesDataType())[7] {
  static const DataType values[] = {
    DataType_NONE,
    DataType_CharactersListData,
    DataType_ErrorData,
    DataType_LoginData,
    DataType_RawData,
    DataType_PlayerData,
    DataType_WeaponData
  };
  return values;
}

inline const char * const *EnumNamesDataType() {
  static const char * const names[8] = {
    "NONE",
    "CharactersListData",
    "ErrorData",
    "LoginData",
    "RawData",
    "PlayerData",
    "WeaponData",
    nullptr
  };
  return names;
}

inline const char *EnumNameDataType(DataType e) {
  if (flatbuffers::IsOutRange(e, DataType_NONE, DataType_WeaponData)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesDataType()[index];
}

template<typename T> struct DataTypeTraits {
  static const DataType enum_value = DataType_NONE;
};

template<> struct DataTypeTraits<CanaryLib::CharactersListData> {
  static const DataType enum_value = DataType_CharactersListData;
};

template<> struct DataTypeTraits<CanaryLib::ErrorData> {
  static const DataType enum_value = DataType_ErrorData;
};

template<> struct DataTypeTraits<CanaryLib::LoginData> {
  static const DataType enum_value = DataType_LoginData;
};

template<> struct DataTypeTraits<CanaryLib::RawData> {
  static const DataType enum_value = DataType_RawData;
};

template<> struct DataTypeTraits<CanaryLib::PlayerData> {
  static const DataType enum_value = DataType_PlayerData;
};

template<> struct DataTypeTraits<CanaryLib::WeaponData> {
  static const DataType enum_value = DataType_WeaponData;
};

bool VerifyDataType(flatbuffers::Verifier &verifier, const void *obj, DataType type);
bool VerifyDataTypeVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct ContentMessage FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ContentMessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DATA_TYPE = 4,
    VT_DATA = 6
  };
  const flatbuffers::Vector<uint8_t> *data_type() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA_TYPE);
  }
  const flatbuffers::Vector<flatbuffers::Offset<void>> *data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<void>> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA_TYPE) &&
           verifier.VerifyVector(data_type()) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           VerifyDataTypeVector(verifier, data(), data_type()) &&
           verifier.EndTable();
  }
};

struct ContentMessageBuilder {
  typedef ContentMessage Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data_type(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data_type) {
    fbb_.AddOffset(ContentMessage::VT_DATA_TYPE, data_type);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<void>>> data) {
    fbb_.AddOffset(ContentMessage::VT_DATA, data);
  }
  explicit ContentMessageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ContentMessage> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ContentMessage>(end);
    return o;
  }
};

inline flatbuffers::Offset<ContentMessage> CreateContentMessage(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data_type = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<void>>> data = 0) {
  ContentMessageBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_data_type(data_type);
  return builder_.Finish();
}

inline flatbuffers::Offset<ContentMessage> CreateContentMessageDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *data_type = nullptr,
    const std::vector<flatbuffers::Offset<void>> *data = nullptr) {
  auto data_type__ = data_type ? _fbb.CreateVector<uint8_t>(*data_type) : 0;
  auto data__ = data ? _fbb.CreateVector<flatbuffers::Offset<void>>(*data) : 0;
  return CanaryLib::CreateContentMessage(
      _fbb,
      data_type__,
      data__);
}

inline bool VerifyDataType(flatbuffers::Verifier &verifier, const void *obj, DataType type) {
  switch (type) {
    case DataType_NONE: {
      return true;
    }
    case DataType_CharactersListData: {
      auto ptr = reinterpret_cast<const CanaryLib::CharactersListData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case DataType_ErrorData: {
      auto ptr = reinterpret_cast<const CanaryLib::ErrorData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case DataType_LoginData: {
      auto ptr = reinterpret_cast<const CanaryLib::LoginData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case DataType_RawData: {
      auto ptr = reinterpret_cast<const CanaryLib::RawData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case DataType_PlayerData: {
      auto ptr = reinterpret_cast<const CanaryLib::PlayerData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case DataType_WeaponData: {
      auto ptr = reinterpret_cast<const CanaryLib::WeaponData *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyDataTypeVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyDataType(
        verifier,  values->Get(i), types->GetEnum<DataType>(i))) {
      return false;
    }
  }
  return true;
}

inline const CanaryLib::ContentMessage *GetContentMessage(const void *buf) {
  return flatbuffers::GetRoot<CanaryLib::ContentMessage>(buf);
}

inline const CanaryLib::ContentMessage *GetSizePrefixedContentMessage(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<CanaryLib::ContentMessage>(buf);
}

inline bool VerifyContentMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<CanaryLib::ContentMessage>(nullptr);
}

inline bool VerifySizePrefixedContentMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<CanaryLib::ContentMessage>(nullptr);
}

inline void FinishContentMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::ContentMessage> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedContentMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::ContentMessage> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace CanaryLib

#endif  // FLATBUFFERS_GENERATED_CONTENTMESSAGE_CANARYLIB_H_
