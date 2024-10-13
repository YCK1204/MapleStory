using Google.FlatBuffers;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using MapleStoryServer.Managers;

namespace MapleStoryServer.Session
{
    public class ClientSession : PacketSession
    {
        public override void OnConnected(EndPoint endPoint)
        {
            var builder = new FlatBufferBuilder(1);

            var ss = builder.CreateString("??");
            C_Monster.StartC_Monster(builder);
            C_Monster.AddName(builder, ss);
            var b = C_Monster.EndC_Monster(builder);
            builder.Finish(b.Value);
            var buf = builder.SizedByteArray();
            ArraySegment<byte> segment = new ArraySegment<byte>(new byte[buf.Length + 4]);

            ushort size = (ushort)(buf.Length + 4);
            BitConverter.TryWriteBytes(new Span<byte>(segment.Array, 0, 2), size);
            BitConverter.TryWriteBytes(new Span<byte>(segment.Array, 2, 4), (ushort)Equipment.C_Monster);
            Buffer.BlockCopy(buf, 0, segment.Array, 4, buf.Length);
            Send(segment.Array);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnDisconnected {endPoint.ToString()}");
        }
        public override void OnRecvPacket(ArraySegment<byte> data)
        {
            Manager.Packet.OnRecvPacket(this, data);
        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"OnSend byte : {numOfBytes}");
        }
    }
}
