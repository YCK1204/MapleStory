// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct SD_CharacterDelete : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static SD_CharacterDelete GetRootAsSD_CharacterDelete(ByteBuffer _bb) { return GetRootAsSD_CharacterDelete(_bb, new SD_CharacterDelete()); }
  public static SD_CharacterDelete GetRootAsSD_CharacterDelete(ByteBuffer _bb, SD_CharacterDelete obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public SD_CharacterDelete __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ulong DbId { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }
  public ulong SessionId { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }
  public ulong CharId { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }

  public static Offset<SD_CharacterDelete> CreateSD_CharacterDelete(FlatBufferBuilder builder,
      ulong db_id = 0,
      ulong session_id = 0,
      ulong char_id = 0) {
    builder.StartTable(3);
    SD_CharacterDelete.AddCharId(builder, char_id);
    SD_CharacterDelete.AddSessionId(builder, session_id);
    SD_CharacterDelete.AddDbId(builder, db_id);
    return SD_CharacterDelete.EndSD_CharacterDelete(builder);
  }

  public static void StartSD_CharacterDelete(FlatBufferBuilder builder) { builder.StartTable(3); }
  public static void AddDbId(FlatBufferBuilder builder, ulong dbId) { builder.AddUlong(0, dbId, 0); }
  public static void AddSessionId(FlatBufferBuilder builder, ulong sessionId) { builder.AddUlong(1, sessionId, 0); }
  public static void AddCharId(FlatBufferBuilder builder, ulong charId) { builder.AddUlong(2, charId, 0); }
  public static Offset<SD_CharacterDelete> EndSD_CharacterDelete(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<SD_CharacterDelete>(o);
  }
}


static public class SD_CharacterDeleteVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyField(tablePos, 4 /*DbId*/, 8 /*ulong*/, 8, false)
      && verifier.VerifyField(tablePos, 6 /*SessionId*/, 8 /*ulong*/, 8, false)
      && verifier.VerifyField(tablePos, 8 /*CharId*/, 8 /*ulong*/, 8, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}
