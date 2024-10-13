using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapleStoryServer.Managers
{
    public class Manager
    {
        #region Singleton
        static Manager _instance = new Manager();
        static Manager Instance { get { return _instance; } }
        PacketManager _packet = new PacketManager();
        public static PacketManager Packet { get { return Instance._packet; } }
        #endregion
    }
}
