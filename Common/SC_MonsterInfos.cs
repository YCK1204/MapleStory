// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct SC_MonsterInfos : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static SC_MonsterInfos GetRootAsSC_MonsterInfos(ByteBuffer _bb) { return GetRootAsSC_MonsterInfos(_bb, new SC_MonsterInfos()); }
  public static SC_MonsterInfos GetRootAsSC_MonsterInfos(ByteBuffer _bb, SC_MonsterInfos obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public SC_MonsterInfos __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public MonsterInfoDetail? MonsterInfos(int j) { int o = __p.__offset(4); return o != 0 ? (MonsterInfoDetail?)(new MonsterInfoDetail()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int MonsterInfosLength { get { int o = __p.__offset(4); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<SC_MonsterInfos> CreateSC_MonsterInfos(FlatBufferBuilder builder,
      VectorOffset monster_infosOffset = default(VectorOffset)) {
    builder.StartTable(1);
    SC_MonsterInfos.AddMonsterInfos(builder, monster_infosOffset);
    return SC_MonsterInfos.EndSC_MonsterInfos(builder);
  }

  public static void StartSC_MonsterInfos(FlatBufferBuilder builder) { builder.StartTable(1); }
  public static void AddMonsterInfos(FlatBufferBuilder builder, VectorOffset monsterInfosOffset) { builder.AddOffset(0, monsterInfosOffset.Value, 0); }
  public static VectorOffset CreateMonsterInfosVector(FlatBufferBuilder builder, Offset<MonsterInfoDetail>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateMonsterInfosVectorBlock(FlatBufferBuilder builder, Offset<MonsterInfoDetail>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateMonsterInfosVectorBlock(FlatBufferBuilder builder, ArraySegment<Offset<MonsterInfoDetail>> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateMonsterInfosVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<Offset<MonsterInfoDetail>>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartMonsterInfosVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<SC_MonsterInfos> EndSC_MonsterInfos(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<SC_MonsterInfos>(o);
  }
}


static public class SC_MonsterInfosVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyVectorOfTables(tablePos, 4 /*MonsterInfos*/, MonsterInfoDetailVerify.Verify, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}
