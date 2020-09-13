// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FLOORDATA_CANARYLIB_H_
#define FLATBUFFERS_GENERATED_FLOORDATA_CANARYLIB_H_

#include "flatbuffers/flatbuffers.h"

#include "creature_data_generated.h"
#include "general_structures_generated.h"
#include "item_data_generated.h"
#include "tile_data_generated.h"

namespace CanaryLib {

struct FloorData;
struct FloorDataBuilder;

struct FloorData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FloorDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TILES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<CanaryLib::TileData>> *tiles() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<CanaryLib::TileData>> *>(VT_TILES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TILES) &&
           verifier.VerifyVector(tiles()) &&
           verifier.VerifyVectorOfTables(tiles()) &&
           verifier.EndTable();
  }
};

struct FloorDataBuilder {
  typedef FloorData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_tiles(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CanaryLib::TileData>>> tiles) {
    fbb_.AddOffset(FloorData::VT_TILES, tiles);
  }
  explicit FloorDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<FloorData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FloorData>(end);
    return o;
  }
};

inline flatbuffers::Offset<FloorData> CreateFloorData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CanaryLib::TileData>>> tiles = 0) {
  FloorDataBuilder builder_(_fbb);
  builder_.add_tiles(tiles);
  return builder_.Finish();
}

inline flatbuffers::Offset<FloorData> CreateFloorDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<CanaryLib::TileData>> *tiles = nullptr) {
  auto tiles__ = tiles ? _fbb.CreateVector<flatbuffers::Offset<CanaryLib::TileData>>(*tiles) : 0;
  return CanaryLib::CreateFloorData(
      _fbb,
      tiles__);
}

inline const CanaryLib::FloorData *GetFloorData(const void *buf) {
  return flatbuffers::GetRoot<CanaryLib::FloorData>(buf);
}

inline const CanaryLib::FloorData *GetSizePrefixedFloorData(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<CanaryLib::FloorData>(buf);
}

inline bool VerifyFloorDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<CanaryLib::FloorData>(nullptr);
}

inline bool VerifySizePrefixedFloorDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<CanaryLib::FloorData>(nullptr);
}

inline void FinishFloorDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::FloorData> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFloorDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<CanaryLib::FloorData> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace CanaryLib

#endif  // FLATBUFFERS_GENERATED_FLOORDATA_CANARYLIB_H_
