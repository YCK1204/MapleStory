using System;
using System.Net.NetworkInformation;
using UnityEngine;
using UnityEngine.UIElements;

public class UIReturnToFirstController : UIBaseController
{
    UINoticeController notice;
    public Button ReturnToBack;
    Button ReturnToFirst;
    Step _step = new Step();
    WorldInfo _worldInfo = new WorldInfo();
    struct Step
    {
        public VisualElement Container;
        public VisualElement Shadow;
        public VisualElement StepImg;
    }
    struct WorldInfo
    {
        public VisualElement Container;
        public VisualElement Shadow;
        public VisualElement WorldInfoImg;
        public Label WorldInfoText;
    }

    protected override void Init()
    {
        base.Init();

        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);

        ReturnToBack = _buttons["ReturnToBack"];
        ReturnToFirst = _buttons["ReturnToFirst"];
        {
            _step.Container = _containers["Step"];
            _step.Shadow = _imgs["Step_Shadow"];
            _step.StepImg = _imgs["Step"];
        }
        {
            _worldInfo.Container = _containers["WorldInfo"];
            _worldInfo.Shadow = _imgs["WorldInfo_Shadow"];
            _worldInfo.WorldInfoImg = _imgs["SelectedWorld"];
            _worldInfo.WorldInfoText = _labels["WorldInfo"];
        }
        ReturnToFirst.RegisterCallback<ClickEvent>((e) =>
        {
            notice.State = UINoticeController.PopupState.ReturnToFirst;
        });
        MoveImgs(UIPrevInGameController.BGState.Login);
    }
    public void MoveImgs(UIPrevInGameController.BGState state, Action callback = null)
    {
        _step.StepImg.ClearClassList();
        string className = state.ToString();
        {
            ReturnToBack.AddToClassList("Hide");
            ReturnToFirst.AddToClassList("Hide");
            _step.Container.AddToClassList("Hide");
            _worldInfo.Container.AddToClassList("Hide");
            Invoke(() =>
            {
                ReturnToBack.AddToClassList("HideLeft");
                ReturnToFirst.AddToClassList("HideLeft");
                _step.Container.AddToClassList("HideLeft");
                _worldInfo.Container.AddToClassList("HideLeft");
                _step.Shadow.AddToClassList("Shadow-Hide");
                _worldInfo.Shadow.AddToClassList("Shadow-Hide");

                _step.StepImg.AddToClassList($"Step-{className}");

                ReturnToBack.style.display = DisplayStyle.Flex;
                _worldInfo.Container.style.display = DisplayStyle.None;
                switch (state)
                {
                    case UIPrevInGameController.BGState.Login:
                        ReturnToBack.style.display = DisplayStyle.None;
                        break;
                    case UIPrevInGameController.BGState.CharacterSelect:
                        _worldInfo.Container.style.display = DisplayStyle.Flex;
                        break;
                }
                if (callback != null)
                    callback.Invoke();
                Invoke(() =>
                {
                    ReturnToBack.RemoveFromClassList("Hide");
                    ReturnToFirst.RemoveFromClassList("Hide");
                    _step.Container.RemoveFromClassList("Hide");
                    _worldInfo.Container.RemoveFromClassList("Hide");
                    ReturnToBack.RemoveFromClassList("HideLeft");
                    ReturnToFirst.RemoveFromClassList("HideLeft");
                    _step.Container.RemoveFromClassList("HideLeft");
                    _worldInfo.Container.RemoveFromClassList("HideLeft");
                    Invoke(() =>
                    {
                        _step.Shadow.RemoveFromClassList("Shadow-Hide");
                        _worldInfo.Shadow.RemoveFromClassList("Shadow-Hide");
                    }, .5f);
                }, 1f);
            }, .75f);
        }
    }
}
