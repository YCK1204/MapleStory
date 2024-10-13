using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ServerCore;
using Google.FlatBuffers;
using DummyClient;

public class PacketHandler
{
    public static void C_MonsterHandler(PacketSession session, ByteBuffer buffer)
    {
        var c = C_Monster.GetRootAsC_Monster(buffer);
        Console.WriteLine(c.Name);
    }
}
