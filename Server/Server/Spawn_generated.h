// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SPAWN_H_
#define FLATBUFFERS_GENERATED_SPAWN_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

#include "Creature_generated.h"
#include "Item_generated.h"
#include "Monster_generated.h"
#include "Player_generated.h"

struct C_Portal;
struct C_PortalBuilder;

struct SC_Portal;
struct SC_PortalBuilder;

struct C_EnterGame;
struct C_EnterGameBuilder;

struct SC_EnterGame;
struct SC_EnterGameBuilder;

struct C_RoomObjects;
struct C_RoomObjectsBuilder;

struct SC_RoomObjects;
struct SC_RoomObjectsBuilder;

struct SC_PSpawn;
struct SC_PSpawnBuilder;

struct C_Despawn;
struct C_DespawnBuilder;

struct SC_PDespawn;
struct SC_PDespawnBuilder;

struct SC_MDespawn;
struct SC_MDespawnBuilder;

struct C_Portal FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef C_PortalBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PORTAL_ID = 4
  };
  uint8_t portal_id() const {
    return GetField<uint8_t>(VT_PORTAL_ID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_PORTAL_ID, 1) &&
           verifier.EndTable();
  }
};

struct C_PortalBuilder {
  typedef C_Portal Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_portal_id(uint8_t portal_id) {
    fbb_.AddElement<uint8_t>(C_Portal::VT_PORTAL_ID, portal_id, 0);
  }
  explicit C_PortalBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<C_Portal> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<C_Portal>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<C_Portal> CreateC_Portal(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t portal_id = 0) {
  C_PortalBuilder builder_(_fbb);
  builder_.add_portal_id(portal_id);
  return builder_.Finish();
}

struct SC_Portal FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SC_PortalBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MAP_ID = 4,
    VT_MY_PLAYER_INFO = 6,
    VT_POSITION = 8
  };
  uint8_t map_id() const {
    return GetField<uint8_t>(VT_MAP_ID, 0);
  }
  const CharacterInfoDetail *my_player_info() const {
    return GetPointer<const CharacterInfoDetail *>(VT_MY_PLAYER_INFO);
  }
  const Position *position() const {
    return GetPointer<const Position *>(VT_POSITION);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_MAP_ID, 1) &&
           VerifyOffset(verifier, VT_MY_PLAYER_INFO) &&
           verifier.VerifyTable(my_player_info()) &&
           VerifyOffset(verifier, VT_POSITION) &&
           verifier.VerifyTable(position()) &&
           verifier.EndTable();
  }
};

struct SC_PortalBuilder {
  typedef SC_Portal Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_map_id(uint8_t map_id) {
    fbb_.AddElement<uint8_t>(SC_Portal::VT_MAP_ID, map_id, 0);
  }
  void add_my_player_info(::flatbuffers::Offset<CharacterInfoDetail> my_player_info) {
    fbb_.AddOffset(SC_Portal::VT_MY_PLAYER_INFO, my_player_info);
  }
  void add_position(::flatbuffers::Offset<Position> position) {
    fbb_.AddOffset(SC_Portal::VT_POSITION, position);
  }
  explicit SC_PortalBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SC_Portal> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SC_Portal>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SC_Portal> CreateSC_Portal(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t map_id = 0,
    ::flatbuffers::Offset<CharacterInfoDetail> my_player_info = 0,
    ::flatbuffers::Offset<Position> position = 0) {
  SC_PortalBuilder builder_(_fbb);
  builder_.add_position(position);
  builder_.add_my_player_info(my_player_info);
  builder_.add_map_id(map_id);
  return builder_.Finish();
}

struct C_EnterGame FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef C_EnterGameBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MAP_ID = 4
  };
  uint8_t map_id() const {
    return GetField<uint8_t>(VT_MAP_ID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_MAP_ID, 1) &&
           verifier.EndTable();
  }
};

struct C_EnterGameBuilder {
  typedef C_EnterGame Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_map_id(uint8_t map_id) {
    fbb_.AddElement<uint8_t>(C_EnterGame::VT_MAP_ID, map_id, 0);
  }
  explicit C_EnterGameBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<C_EnterGame> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<C_EnterGame>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<C_EnterGame> CreateC_EnterGame(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t map_id = 0) {
  C_EnterGameBuilder builder_(_fbb);
  builder_.add_map_id(map_id);
  return builder_.Finish();
}

struct SC_EnterGame FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SC_EnterGameBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MAP_ID = 4,
    VT_MY_PLAYER_INFO = 6,
    VT_POSITION = 8
  };
  uint8_t map_id() const {
    return GetField<uint8_t>(VT_MAP_ID, 0);
  }
  const CharacterInfoDetail *my_player_info() const {
    return GetPointer<const CharacterInfoDetail *>(VT_MY_PLAYER_INFO);
  }
  const Position *position() const {
    return GetPointer<const Position *>(VT_POSITION);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_MAP_ID, 1) &&
           VerifyOffset(verifier, VT_MY_PLAYER_INFO) &&
           verifier.VerifyTable(my_player_info()) &&
           VerifyOffset(verifier, VT_POSITION) &&
           verifier.VerifyTable(position()) &&
           verifier.EndTable();
  }
};

struct SC_EnterGameBuilder {
  typedef SC_EnterGame Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_map_id(uint8_t map_id) {
    fbb_.AddElement<uint8_t>(SC_EnterGame::VT_MAP_ID, map_id, 0);
  }
  void add_my_player_info(::flatbuffers::Offset<CharacterInfoDetail> my_player_info) {
    fbb_.AddOffset(SC_EnterGame::VT_MY_PLAYER_INFO, my_player_info);
  }
  void add_position(::flatbuffers::Offset<Position> position) {
    fbb_.AddOffset(SC_EnterGame::VT_POSITION, position);
  }
  explicit SC_EnterGameBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SC_EnterGame> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SC_EnterGame>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SC_EnterGame> CreateSC_EnterGame(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t map_id = 0,
    ::flatbuffers::Offset<CharacterInfoDetail> my_player_info = 0,
    ::flatbuffers::Offset<Position> position = 0) {
  SC_EnterGameBuilder builder_(_fbb);
  builder_.add_position(position);
  builder_.add_my_player_info(my_player_info);
  builder_.add_map_id(map_id);
  return builder_.Finish();
}

struct C_RoomObjects FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef C_RoomObjectsBuilder Builder;
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct C_RoomObjectsBuilder {
  typedef C_RoomObjects Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  explicit C_RoomObjectsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<C_RoomObjects> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<C_RoomObjects>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<C_RoomObjects> CreateC_RoomObjects(
    ::flatbuffers::FlatBufferBuilder &_fbb) {
  C_RoomObjectsBuilder builder_(_fbb);
  return builder_.Finish();
}

struct SC_RoomObjects FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SC_RoomObjectsBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PLAYERS = 4,
    VT_ITEMS = 6
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<PlayerInfo>> *players() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<PlayerInfo>> *>(VT_PLAYERS);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<Coin>> *items() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<Coin>> *>(VT_ITEMS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_PLAYERS) &&
           verifier.VerifyVector(players()) &&
           verifier.VerifyVectorOfTables(players()) &&
           VerifyOffset(verifier, VT_ITEMS) &&
           verifier.VerifyVector(items()) &&
           verifier.VerifyVectorOfTables(items()) &&
           verifier.EndTable();
  }
};

struct SC_RoomObjectsBuilder {
  typedef SC_RoomObjects Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_players(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<PlayerInfo>>> players) {
    fbb_.AddOffset(SC_RoomObjects::VT_PLAYERS, players);
  }
  void add_items(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Coin>>> items) {
    fbb_.AddOffset(SC_RoomObjects::VT_ITEMS, items);
  }
  explicit SC_RoomObjectsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SC_RoomObjects> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SC_RoomObjects>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SC_RoomObjects> CreateSC_RoomObjects(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<PlayerInfo>>> players = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Coin>>> items = 0) {
  SC_RoomObjectsBuilder builder_(_fbb);
  builder_.add_items(items);
  builder_.add_players(players);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<SC_RoomObjects> CreateSC_RoomObjectsDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<::flatbuffers::Offset<PlayerInfo>> *players = nullptr,
    const std::vector<::flatbuffers::Offset<Coin>> *items = nullptr) {
  auto players__ = players ? _fbb.CreateVector<::flatbuffers::Offset<PlayerInfo>>(*players) : 0;
  auto items__ = items ? _fbb.CreateVector<::flatbuffers::Offset<Coin>>(*items) : 0;
  return CreateSC_RoomObjects(
      _fbb,
      players__,
      items__);
}

struct SC_PSpawn FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SC_PSpawnBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PLAYER_INFO = 4
  };
  const PlayerInfo *player_info() const {
    return GetPointer<const PlayerInfo *>(VT_PLAYER_INFO);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_PLAYER_INFO) &&
           verifier.VerifyTable(player_info()) &&
           verifier.EndTable();
  }
};

struct SC_PSpawnBuilder {
  typedef SC_PSpawn Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_player_info(::flatbuffers::Offset<PlayerInfo> player_info) {
    fbb_.AddOffset(SC_PSpawn::VT_PLAYER_INFO, player_info);
  }
  explicit SC_PSpawnBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SC_PSpawn> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SC_PSpawn>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SC_PSpawn> CreateSC_PSpawn(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<PlayerInfo> player_info = 0) {
  SC_PSpawnBuilder builder_(_fbb);
  builder_.add_player_info(player_info);
  return builder_.Finish();
}

struct C_Despawn FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef C_DespawnBuilder Builder;
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct C_DespawnBuilder {
  typedef C_Despawn Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  explicit C_DespawnBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<C_Despawn> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<C_Despawn>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<C_Despawn> CreateC_Despawn(
    ::flatbuffers::FlatBufferBuilder &_fbb) {
  C_DespawnBuilder builder_(_fbb);
  return builder_.Finish();
}

struct SC_PDespawn FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SC_PDespawnBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4
  };
  uint64_t id() const {
    return GetField<uint64_t>(VT_ID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_ID, 8) &&
           verifier.EndTable();
  }
};

struct SC_PDespawnBuilder {
  typedef SC_PDespawn Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_id(uint64_t id) {
    fbb_.AddElement<uint64_t>(SC_PDespawn::VT_ID, id, 0);
  }
  explicit SC_PDespawnBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SC_PDespawn> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SC_PDespawn>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SC_PDespawn> CreateSC_PDespawn(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t id = 0) {
  SC_PDespawnBuilder builder_(_fbb);
  builder_.add_id(id);
  return builder_.Finish();
}

struct SC_MDespawn FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SC_MDespawnBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_COIN = 6
  };
  const ::flatbuffers::Vector<uint64_t> *id() const {
    return GetPointer<const ::flatbuffers::Vector<uint64_t> *>(VT_ID);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<Coin>> *coin() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<Coin>> *>(VT_COIN);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyVector(id()) &&
           VerifyOffset(verifier, VT_COIN) &&
           verifier.VerifyVector(coin()) &&
           verifier.VerifyVectorOfTables(coin()) &&
           verifier.EndTable();
  }
};

struct SC_MDespawnBuilder {
  typedef SC_MDespawn Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_id(::flatbuffers::Offset<::flatbuffers::Vector<uint64_t>> id) {
    fbb_.AddOffset(SC_MDespawn::VT_ID, id);
  }
  void add_coin(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Coin>>> coin) {
    fbb_.AddOffset(SC_MDespawn::VT_COIN, coin);
  }
  explicit SC_MDespawnBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SC_MDespawn> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SC_MDespawn>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SC_MDespawn> CreateSC_MDespawn(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<uint64_t>> id = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Coin>>> coin = 0) {
  SC_MDespawnBuilder builder_(_fbb);
  builder_.add_coin(coin);
  builder_.add_id(id);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<SC_MDespawn> CreateSC_MDespawnDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint64_t> *id = nullptr,
    const std::vector<::flatbuffers::Offset<Coin>> *coin = nullptr) {
  auto id__ = id ? _fbb.CreateVector<uint64_t>(*id) : 0;
  auto coin__ = coin ? _fbb.CreateVector<::flatbuffers::Offset<Coin>>(*coin) : 0;
  return CreateSC_MDespawn(
      _fbb,
      id__,
      coin__);
}

#endif  // FLATBUFFERS_GENERATED_SPAWN_H_
