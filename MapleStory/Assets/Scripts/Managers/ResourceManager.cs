using UnityEngine;


public class ResourceManager : IManager
{
    public void Clear()
    {
    }
    public void Init()
    {
    }
    public GameObject Instantiate(GameObject go)
    {
        return Instantiate(go);
    }
    public GameObject Instantiate(string path)
    {
        GameObject go = Load<GameObject>(path);
        if (go != null)
            return Instantiate(go);
        return null;
    }
    public T Load<T>(string path) where T : Object
    {
        T t = Resources.Load<T>(path);
        return t;
    }
}