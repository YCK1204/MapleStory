using UnityEngine;

public abstract class BaseController : BaseMonobehaviour
{
    void Start()
    {
        Init();
    }
    protected abstract void Init();
    protected abstract void UpdateController();
    protected abstract void UpdateAnimation();
}
