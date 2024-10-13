using System.IO;
using System.Text;
using System.Windows;

namespace PacketGenerator
{
    public class Program
    {
        static string path = "./Protocol.fbs";
        static string ClientStr = "";
        static string ServerStr = "";
        static void Main(string[] args)
        {
            if (args.Length == 1)
                path = args[0];

            using (StreamReader sr = new StreamReader(path, encoding: Encoding.UTF8))
            {
                string clientRegister = "";
                string serverRegister = "";
                while (true)
                {
                    string line = sr.ReadLine();
                    if (line == null)
                        break;
                    line = line.Trim();
                    if (line.StartsWith("union") == true)
                    {
                        while (true)
                        {
                            line = sr.ReadLine();
                            if (line == null || line.Equals("}"))
                                break;
                            line = line.Trim();
                            if (line.Length == 0)
                                continue;
                            if (line.Equals("}"))
                                break;
                            line = line.Substring(0, line.Length - 1);
                            if (line.StartsWith("C_"))
                            {
                                clientRegister += String.Format(PacketFormat.PMRegister,  line) + "\n";
                            }
                            else if (line.StartsWith("S_"))
                            {
                                serverRegister += String.Format(PacketFormat.PMRegister, line) + "\n";
                            }
                            else
                            {
                                Console.Error.WriteLine("Error : Packet Name is not valid");
                                List<string> list = null;
                                list.Add("");
                            }
                        }
                        break;
                    }
                }
                ClientStr = String.Format(PacketFormat.PMTotal, clientRegister);
                ServerStr = String.Format(PacketFormat.PMTotal, serverRegister);
                File.WriteAllText("./Client/PacketManager.cs", ClientStr);
                File.WriteAllText("./Server/PacketManager.cs", ServerStr);
            }
        }
    }
}