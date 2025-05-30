// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct SC_MoveStart : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static SC_MoveStart GetRootAsSC_MoveStart(ByteBuffer _bb) { return GetRootAsSC_MoveStart(_bb, new SC_MoveStart()); }
  public static SC_MoveStart GetRootAsSC_MoveStart(ByteBuffer _bb, SC_MoveStart obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public SC_MoveStart __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ulong Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }
  public MoveDirection Dir { get { int o = __p.__offset(6); return o != 0 ? (MoveDirection)__p.bb.Get(o + __p.bb_pos) : MoveDirection.NONE; } }
  public float X { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float Y { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }

  public static Offset<SC_MoveStart> CreateSC_MoveStart(FlatBufferBuilder builder,
      ulong id = 0,
      MoveDirection dir = MoveDirection.NONE,
      float x = 0.0f,
      float y = 0.0f) {
    builder.StartTable(4);
    SC_MoveStart.AddId(builder, id);
    SC_MoveStart.AddY(builder, y);
    SC_MoveStart.AddX(builder, x);
    SC_MoveStart.AddDir(builder, dir);
    return SC_MoveStart.EndSC_MoveStart(builder);
  }

  public static void StartSC_MoveStart(FlatBufferBuilder builder) { builder.StartTable(4); }
  public static void AddId(FlatBufferBuilder builder, ulong id) { builder.AddUlong(0, id, 0); }
  public static void AddDir(FlatBufferBuilder builder, MoveDirection dir) { builder.AddByte(1, (byte)dir, 0); }
  public static void AddX(FlatBufferBuilder builder, float x) { builder.AddFloat(2, x, 0.0f); }
  public static void AddY(FlatBufferBuilder builder, float y) { builder.AddFloat(3, y, 0.0f); }
  public static Offset<SC_MoveStart> EndSC_MoveStart(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<SC_MoveStart>(o);
  }
}


static public class SC_MoveStartVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyField(tablePos, 4 /*Id*/, 8 /*ulong*/, 8, false)
      && verifier.VerifyField(tablePos, 6 /*Dir*/, 1 /*MoveDirection*/, 1, false)
      && verifier.VerifyField(tablePos, 8 /*X*/, 4 /*float*/, 4, false)
      && verifier.VerifyField(tablePos, 10 /*Y*/, 4 /*float*/, 4, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}
