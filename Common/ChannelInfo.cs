// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct ChannelInfo : IFlatbufferObject
{
  private Struct __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public void __init(int _i, ByteBuffer _bb) { __p = new Struct(_i, _bb); }
  public ChannelInfo __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ushort UserCount { get { return __p.bb.GetUshort(__p.bb_pos + 0); } }
  public byte Id { get { return __p.bb.Get(__p.bb_pos + 2); } }

  public static Offset<ChannelInfo> CreateChannelInfo(FlatBufferBuilder builder, ushort UserCount, byte Id) {
    builder.Prep(2, 4);
    builder.Pad(1);
    builder.PutByte(Id);
    builder.PutUshort(UserCount);
    return new Offset<ChannelInfo>(builder.Offset);
  }
}
