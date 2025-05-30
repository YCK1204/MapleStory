// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct CharacterAbility : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static CharacterAbility GetRootAsCharacterAbility(ByteBuffer _bb) { return GetRootAsCharacterAbility(_bb, new CharacterAbility()); }
  public static CharacterAbility GetRootAsCharacterAbility(ByteBuffer _bb, CharacterAbility obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CharacterAbility __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ushort STR { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }
  public ushort DEX { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }
  public ushort INT { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }
  public ushort LUK { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }

  public static Offset<CharacterAbility> CreateCharacterAbility(FlatBufferBuilder builder,
      ushort STR = 0,
      ushort DEX = 0,
      ushort INT = 0,
      ushort LUK = 0) {
    builder.StartTable(4);
    CharacterAbility.AddLUK(builder, LUK);
    CharacterAbility.AddINT(builder, INT);
    CharacterAbility.AddDEX(builder, DEX);
    CharacterAbility.AddSTR(builder, STR);
    return CharacterAbility.EndCharacterAbility(builder);
  }

  public static void StartCharacterAbility(FlatBufferBuilder builder) { builder.StartTable(4); }
  public static void AddSTR(FlatBufferBuilder builder, ushort sTR) { builder.AddUshort(0, sTR, 0); }
  public static void AddDEX(FlatBufferBuilder builder, ushort dEX) { builder.AddUshort(1, dEX, 0); }
  public static void AddINT(FlatBufferBuilder builder, ushort iNT) { builder.AddUshort(2, iNT, 0); }
  public static void AddLUK(FlatBufferBuilder builder, ushort lUK) { builder.AddUshort(3, lUK, 0); }
  public static Offset<CharacterAbility> EndCharacterAbility(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<CharacterAbility>(o);
  }
}


static public class CharacterAbilityVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyField(tablePos, 4 /*STR*/, 2 /*ushort*/, 2, false)
      && verifier.VerifyField(tablePos, 6 /*DEX*/, 2 /*ushort*/, 2, false)
      && verifier.VerifyField(tablePos, 8 /*INT*/, 2 /*ushort*/, 2, false)
      && verifier.VerifyField(tablePos, 10 /*LUK*/, 2 /*ushort*/, 2, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}
