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

    public static bool IsInLengthRange(this string input, int minLength, int maxLength)
    {
        if (input == null) return false;
        int length = input.Length;
        return length >= minLength && length <= maxLength;
    }
}
