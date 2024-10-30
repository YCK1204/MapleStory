using UnityEngine;

public static class Extension
{
    public static bool isAlNum(this string str)
    {
        foreach (char c in str)
        {
            if (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
                continue;
            return false;
        }
        return true;
    }
}
