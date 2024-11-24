using System.IO;
using System.Text;
using System.Windows;

namespace PacketGenerator
{
    public class Program
    {
        static string path = "./Protocol.fbs";
        static string ClientStr = "";


        static string ServerHeaderStr = "";
        static string ServerSourceStr = "";

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
                            string packetType = line.Substring(0, 2);
                            switch (packetType)
                            {
                                case "S_":
                                    clientRegister += String.Format(PacketFormat_CSharp.PMRegister, line) + "\n\t\t";

                                    break;
                                case "C_":
                                    serverRegister += String.Format(PacketFormat_CPP.PMRegister, line) + "\n\t\t";
                                    break;
                                default:
                                    Console.Error.WriteLine("Error : Packet Name is not valid");
                                    List<string> list = null;
                                    list.Add("");
                                    break;
                            }
                        }
                        break;
                    }
                }
                ClientStr = String.Format(PacketFormat_CSharp.PMTotal, clientRegister);
                File.WriteAllText("./Client/PacketManager.cs", ClientStr);

                ServerHeaderStr = PacketFormat_CPP.Header.HTotal;
                ServerSourceStr = String.Format(PacketFormat_CPP.Source.STotal, serverRegister);
                File.WriteAllText("PacketManager.h", ServerHeaderStr);
                File.WriteAllText("PacketManager.cpp", ServerSourceStr);
            }
        }
    }
}