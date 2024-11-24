using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public class PacketHandler
{
    public static void S_TestHandler(PacketSession session, ByteBuffer buffer)
    {
        var test = S_Test.GetRootAsS_Test(buffer);
        Debug.Log($"str : {test.Str}, int : {test.Test}");
    }
}
