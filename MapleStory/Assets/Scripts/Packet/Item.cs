using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_DespawnItemHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_DespawnItem.GetRootAsSC_DespawnItem(buffer);

        var len = pkt.ItemsLength;
        for (var i = 0; i < len; i++)
            Manager.Object.RemoveObject(pkt.Items(i));
    }
    public static void SC_CollectCoinHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CollectCoin.GetRootAsSC_CollectCoin(buffer);

        Manager.Object.RemoveObject(pkt.Id);
    }
}
