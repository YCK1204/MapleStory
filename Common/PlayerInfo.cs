// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct PlayerInfo : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static PlayerInfo GetRootAsPlayerInfo(ByteBuffer _bb) { return GetRootAsPlayerInfo(_bb, new PlayerInfo()); }
  public static PlayerInfo GetRootAsPlayerInfo(ByteBuffer _bb, PlayerInfo obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public PlayerInfo __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public CharacterInfo? CharInfo { get { int o = __p.__offset(4); return o != 0 ? (CharacterInfo?)(new CharacterInfo()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public Position? Position { get { int o = __p.__offset(6); return o != 0 ? (Position?)(new Position()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }

  public static Offset<PlayerInfo> CreatePlayerInfo(FlatBufferBuilder builder,
      Offset<CharacterInfo> char_infoOffset = default(Offset<CharacterInfo>),
      Offset<Position> positionOffset = default(Offset<Position>)) {
    builder.StartTable(2);
    PlayerInfo.AddPosition(builder, positionOffset);
    PlayerInfo.AddCharInfo(builder, char_infoOffset);
    return PlayerInfo.EndPlayerInfo(builder);
  }

  public static void StartPlayerInfo(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddCharInfo(FlatBufferBuilder builder, Offset<CharacterInfo> charInfoOffset) { builder.AddOffset(0, charInfoOffset.Value, 0); }
  public static void AddPosition(FlatBufferBuilder builder, Offset<Position> positionOffset) { builder.AddOffset(1, positionOffset.Value, 0); }
  public static Offset<PlayerInfo> EndPlayerInfo(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<PlayerInfo>(o);
  }
}


static public class PlayerInfoVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTable(tablePos, 4 /*CharInfo*/, CharacterInfoVerify.Verify, false)
      && verifier.VerifyTable(tablePos, 6 /*Position*/, PositionVerify.Verify, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}
