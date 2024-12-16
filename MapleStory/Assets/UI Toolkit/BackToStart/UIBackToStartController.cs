using UnityEngine;
using UnityEngine.UIElements;

public class UIBackToStartController : UIBaseController
{
    Button back;
    Button backToStart;
    UINoticeController notice;
    protected override void Init()
    {
        base.Init();
        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);

        back = _buttons["Back"];
        backToStart = _buttons["BackToStart"];
        back.RegisterCallback<ClickEvent>(BackToPrevScene);
        backToStart.RegisterCallback<ClickEvent>(BackToStartScene);
    }
    private void BackToPrevScene(ClickEvent e)
    {
        Manager.Scene.LoadScene(Manager.Scene.CurScene.PrevSceneName);
    }
    private void BackToStartScene(ClickEvent e)
    {
        notice.State = UINoticeController.PopupState.BackToFirst;
    }
}
