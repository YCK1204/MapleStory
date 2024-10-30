using UnityEngine;
using UnityEngine.UIElements;

public class BaseScene : BaseMonobehaviour
{
    protected UIDocument document;
    private void Start()
    {
        Init();
    }
    protected virtual void Init()
    {
        document = GetComponent<UIDocument>();
    }

    protected virtual void Clear()
    {

    }
}
