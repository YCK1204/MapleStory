using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class LoginScene : BaseScene
{
    VisualElement LoadingCircle;
    Coroutine _loading = null;
    protected override void Clear()
    {
    }
    protected override void Init()
    {
        base.Init();
        root = GetComponent<UIDocument>().rootVisualElement;
        LoadingCircle = root.Q<VisualElement>("Img_LoadingCircle");
        StartLoadingAnim();
    }

    public void StartLoadingAnim()
    {
        _loading = StartCoroutine(CoLoading());
    }

    IEnumerator CoLoading()
    {
        int i = 0;
        var img = LoadingCircle.resolvedStyle.backgroundImage;
        while (true)
        {
            LoadingCircle.style.backgroundImage = Resources.Load<Texture2D>($"Imgs/Login/Notice/Loading/Notice.Loading.circle.{i++}");
            yield return new WaitForSeconds(.1f);
            if (i == 15)
                i = 0;
        }
    }
}
