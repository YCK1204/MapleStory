// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct C_CheckName : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static C_CheckName GetRootAsC_CheckName(ByteBuffer _bb) { return GetRootAsC_CheckName(_bb, new C_CheckName()); }
  public static C_CheckName GetRootAsC_CheckName(ByteBuffer _bb, C_CheckName obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public C_CheckName __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public string Name { get { int o = __p.__offset(4); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span<byte>(4, 1); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(4); }

  public static Offset<C_CheckName> CreateC_CheckName(FlatBufferBuilder builder,
      StringOffset nameOffset = default(StringOffset)) {
    builder.StartTable(1);
    C_CheckName.AddName(builder, nameOffset);
    return C_CheckName.EndC_CheckName(builder);
  }

  public static void StartC_CheckName(FlatBufferBuilder builder) { builder.StartTable(1); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(0, nameOffset.Value, 0); }
  public static Offset<C_CheckName> EndC_CheckName(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<C_CheckName>(o);
  }
}


static public class C_CheckNameVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyString(tablePos, 4 /*Name*/, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}
