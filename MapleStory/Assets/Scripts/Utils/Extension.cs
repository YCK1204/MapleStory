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
    public static bool HasLayer(this GameObject go, params string[] layerName)
    {
        return ((1 << go.layer) & LayerMask.GetMask(layerName)) != 0;
    }
    public static void FlipXLeft(this Transform transform)
    {
        transform.localScale = new Vector3(Mathf.Abs(transform.localScale.x), transform.localScale.y, transform.localScale.z);
    }
    public static void FlipXRight(this Transform transform)
    {
        transform.localScale = new Vector3(-Mathf.Abs(transform.localScale.x), transform.localScale.y, transform.localScale.z);
    }
    public static bool IsInRange(this Vector3 position1, Position position2, float range)
    {
        return (position2.X + 1 > position1.x && position2.X - 1 < position1.x);
    }
    public static Vector3Int ToVector3Int(this Vector3 position)
    {
        return new Vector3Int((int)position.x, (int)position.y, (int)position.z);
    }
}
