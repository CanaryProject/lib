// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TILEDATA_CANARYLIB_H_
#define FLATBUFFERS_GENERATED_TILEDATA_CANARYLIB_H_

#include "flatbuffers/flatbuffers.h"

#include "creature_data_generated.h"
#include "general_structures_generated.h"
#include "item_data_generated.h"

namespace CanaryLib {

struct TileData;
struct TileDataBuilder;

struct TileData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef TileDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_POS = 4,
    VT_LOCAL_PLAYER = 6,
    VT_CREATURES = 8,
    VT_GROUND = 10,
    VT_BOTTOM_ITEMS = 12,
    VT_TOP_ITEMS = 14,
    VT_IS_CENTRAL = 16,
    VT_CLEAN = 18
  };
  const CanaryLib::Position *pos() const {
    return GetStruct<const CanaryLib::Position *>(VT_POS);
  }
  const CanaryLib::CreatureData *local_player() const {
    return GetPointer<const CanaryLib::CreatureData *>(VT_LOCAL_PLAYER);
  }
  const flatbuffers::Vector<flatbuffers::Offset<CanaryLib::CreatureData>> *creatures() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<CanaryLib::CreatureData>> *>(VT_CREATURES);
  }
  const CanaryLib::ItemData *ground() const {
    return GetStruct<const CanaryLib::ItemData *>(VT_GROUND);
  }
  const flatbuffers::Vector<const CanaryLib::ItemData *> *bottom_items() const {
    return GetPointer<const flatbuffers::Vector<const CanaryLib::ItemData *> *>(VT_BOTTOM_ITEMS);
  }
  const flatbuffers::Vector<const CanaryLib::ItemData *> *top_items() const {
    return GetPointer<const flatbuffers::Vector<const CanaryLib::ItemData *> *>(VT_TOP_ITEMS);
  }
  bool is_central() const {
    return GetField<uint8_t>(VT_IS_CENTRAL, 0) != 0;
  }
  bool clean() const {
    return GetField<uint8_t>(VT_CLEAN, 1) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<CanaryLib::Position>(verifier, VT_POS) &&
           VerifyOffset(verifier, VT_LOCAL_PLAYER) &&
           verifier.VerifyTable(local_player()) &&
           VerifyOffset(verifier, VT_CREATURES) &&
           verifier.VerifyVector(creatures()) &&
           verifier.VerifyVectorOfTables(creatures()) &&
           VerifyField<CanaryLib::ItemData>(verifier, VT_GROUND) &&
           VerifyOffset(verifier, VT_BOTTOM_ITEMS) &&
           verifier.VerifyVector(bottom_items()) &&
           VerifyOffset(verifier, VT_TOP_ITEMS) &&
           verifier.VerifyVector(top_items()) &&
           VerifyField<uint8_t>(verifier, VT_IS_CENTRAL) &&
           VerifyField<uint8_t>(verifier, VT_CLEAN) &&
           verifier.EndTable();
  }
};

struct TileDataBuilder {
  typedef TileData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_pos(const CanaryLib::Position *pos) {
    fbb_.AddStruct(TileData::VT_POS, pos);
  }
  void add_local_player(flatbuffers::Offset<CanaryLib::CreatureData> local_player) {
    fbb_.AddOffset(TileData::VT_LOCAL_PLAYER, local_player);
  }
  void add_creatures(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CanaryLib::CreatureData>>> creatures) {
    fbb_.AddOffset(TileData::VT_CREATURES, creatures);
  }
  void add_ground(const CanaryLib::ItemData *ground) {
    fbb_.AddStruct(TileData::VT_GROUND, ground);
  }
  void add_bottom_items(flatbuffers::Offset<flatbuffers::Vector<const CanaryLib::ItemData *>> bottom_items) {
    fbb_.AddOffset(TileData::VT_BOTTOM_ITEMS, bottom_items);
  }
  void add_top_items(flatbuffers::Offset<flatbuffers::Vector<const CanaryLib::ItemData *>> top_items) {
    fbb_.AddOffset(TileData::VT_TOP_ITEMS, top_items);
  }
  void add_is_central(bool is_central) {
    fbb_.AddElement<uint8_t>(TileData::VT_IS_CENTRAL, static_cast<uint8_t>(is_central), 0);
  }
  void add_clean(bool clean) {
    fbb_.AddElement<uint8_t>(TileData::VT_CLEAN, static_cast<uint8_t>(clean), 1);
  }
  explicit TileDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<TileData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<TileData>(end);
    return o;
  }
};

inline flatbuffers::Offset<TileData> CreateTileData(
    flatbuffers::FlatBufferBuilder &_fbb,
    const CanaryLib::Position *pos = 0,
    flatbuffers::Offset<CanaryLib::CreatureData> local_player = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CanaryLib::CreatureData>>> creatures = 0,
    const CanaryLib::ItemData *ground = 0,
    flatbuffers::Offset<flatbuffers::Vector<const CanaryLib::ItemData *>> bottom_items = 0,
    flatbuffers::Offset<flatbuffers::Vector<const CanaryLib::ItemData *>> top_items = 0,
    bool is_central = false,
    bool clean = true) {
  TileDataBuilder builder_(_fbb);
  builder_.add_top_items(top_items);
  builder_.add_bottom_items(bottom_items);
  builder_.add_ground(ground);
  builder_.add_creatures(creatures);
  builder_.add_local_player(local_player);
  builder_.add_pos(pos);
  builder_.add_clean(clean);
  builder_.add_is_central(is_central);
  return builder_.Finish();
}

inline flatbuffers::Offset<TileData> CreateTileDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const CanaryLib::Position *pos = 0,
    flatbuffers::Offset<CanaryLib::CreatureData> local_player = 0,
    const std::vector<flatbuffers::Offset<CanaryLib::CreatureData>> *creatures = nullptr,
    const CanaryLib::ItemData *ground = 0,
    const std::vector<CanaryLib::ItemData> *bottom_items = nullptr,
    const std::vector<CanaryLib::ItemData> *top_items = nullptr,
    bool is_central = false,
    bool clean = true) {
  auto creatures__ = creatures ? _fbb.CreateVector<flatbuffers::Offset<CanaryLib::CreatureData>>(*creatures) : 0;
  auto bottom_items__ = bottom_items ? _fbb.CreateVectorOfStructs<CanaryLib::ItemData>(*bottom_items) : 0;
  auto top_items__ = top_items ? _fbb.CreateVectorOfStructs<CanaryLib::ItemData>(*top_items) : 0;
  return CanaryLib::CreateTileData(
      _fbb,
      pos,
      local_player,
      creatures__,
      ground,
      bottom_items__,
      top_items__,
      is_central,
      clean);
}

inline const CanaryLib::TileData *GetTileData(const void *buf) {
  return flatbuffers::GetRoot<CanaryLib::TileData>(buf);
}

inline const CanaryLib::TileData *GetSizePrefixedTileData(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<CanaryLib::TileData>(buf);
}

inline bool VerifyTileDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<CanaryLib::TileData>(nullptr);
}

inline bool VerifySizePrefixedTileDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<CanaryLib::TileData>(nullptr);
}

inline void FinishTileDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::TileData> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTileDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::TileData> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace CanaryLib

#endif  // FLATBUFFERS_GENERATED_TILEDATA_CANARYLIB_H_