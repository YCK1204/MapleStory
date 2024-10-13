using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public class Utils
    {

        public static void LogCurrentLocation([CallerFilePath] string filePath = "",
                                          [CallerLineNumber] int lineNumber = 0,
                                          [CallerMemberName] string memberName = "",
                                          Action<string?> printAction = null)
        {
            printAction ??= Console.WriteLine;
            printAction($"File: {filePath}");
            printAction($"Line: {lineNumber}");
            printAction($"Method: {memberName}");
        }
    }
}
