using UnityEngine;

public abstract class BaseController : MonoBehaviour
{
    void Start()
    {
        Init();
    }
    protected abstract void Init();
    protected abstract void UpdateController();
    protected abstract void UpdateAnimation();
}
