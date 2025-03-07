using UnityEngine;

public class Util
{
    public static T FindChild<T>(Transform t, bool recursive = false, string name = null) where T : Component
    {
        if (recursive)
        {
            T[] childs = t.GetComponentsInChildren<T>();
            foreach (T child in childs)
            {
                if (name == null)
                    return child;
                if (child.name == name)
                    return child;
            }
        }
        else
        {
            int childCount = t.childCount;
            for (int i = 0; i < childCount; i++)
            {
                Transform child = t.GetChild(i);
                T component = child.GetComponent<T>();
                if (component != null)
                {
                    if (name == null)
                        return component;
                    if (component.name == name)
                        return component;
                }
            }
        }
        return null;
    }
}