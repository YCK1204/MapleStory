// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct C_Despawn : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static C_Despawn GetRootAsC_Despawn(ByteBuffer _bb) { return GetRootAsC_Despawn(_bb, new C_Despawn()); }
  public static C_Despawn GetRootAsC_Despawn(ByteBuffer _bb, C_Despawn obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public C_Despawn __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }


  public static void StartC_Despawn(FlatBufferBuilder builder) { builder.StartTable(0); }
  public static Offset<C_Despawn> EndC_Despawn(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<C_Despawn>(o);
  }
}


static public class C_DespawnVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTableEnd(tablePos);
  }
}
