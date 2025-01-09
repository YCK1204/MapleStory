using System;
using System.Net.NetworkInformation;
using UnityEngine;
using UnityEngine.UIElements;

public class UIReturnToFirstController : UIBaseController
{
    UINoticeController notice;
    public class Step
    {
        public VisualElement ContainerStep;
        public VisualElement ImgStep_Shadow;
        public VisualElement ImgStep;
    }
    public class WorldInfo
    {
        public VisualElement ContainerWorldInfo;
        public VisualElement ImgWorldInfo_Shadow;
        public VisualElement ImgSelectedWorld;
        public Label LabelWorldInfo;
    }
    public class ReturnToFirst
    {
        public Step step = new Step();
        public WorldInfo worldInfo = new WorldInfo();
        public Button ButtonReturnToBack;
        public Button ButtonReturnToFirst;
    }
    public ReturnToFirst _returnToFirst = new ReturnToFirst();
    protected override void Init()
    {
        base.Init();

        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);

        AssignElements(_returnToFirst);
        _returnToFirst.ButtonReturnToFirst.RegisterCallback<ClickEvent>((e) =>
        {
            notice.State = UINoticeController.PopupState.ReturnToFirst;
        });
        MoveImgs(UIPrevInGameController.BGState.Login);
    }
    public void MoveImgs(UIPrevInGameController.BGState state, Action callback = null)
    {
        _returnToFirst.step.ImgStep.ClearClassList();
        string className = state.ToString();
        {
            _returnToFirst.ButtonReturnToBack.AddToClassList("Hide");
            _returnToFirst.ButtonReturnToFirst.AddToClassList("Hide");
            _returnToFirst.step.ContainerStep.AddToClassList("Hide");
            _returnToFirst.worldInfo.ContainerWorldInfo.AddToClassList("Hide");
            Invoke(() =>
            {
                
                _returnToFirst.ButtonReturnToBack.AddToClassList("HideLeft");
                _returnToFirst.ButtonReturnToFirst.AddToClassList("HideLeft");
                _returnToFirst.step.ContainerStep.AddToClassList("HideLeft");
                _returnToFirst.worldInfo.ContainerWorldInfo.AddToClassList("HideLeft");

                _returnToFirst.step.ImgStep_Shadow.AddToClassList("Shadow-Hide");
                _returnToFirst.worldInfo.ImgWorldInfo_Shadow.AddToClassList("Shadow-Hide");

                _returnToFirst.step.ImgStep.AddToClassList($"Step-{className}");

                _returnToFirst.ButtonReturnToBack.style.display = DisplayStyle.Flex;
               _returnToFirst.worldInfo.ContainerWorldInfo.style.display = DisplayStyle.None;
                switch (state)
                {
                    case UIPrevInGameController.BGState.Login:
                        _returnToFirst.ButtonReturnToBack.style.display = DisplayStyle.None;
                        break;
                    case UIPrevInGameController.BGState.CharacterSelect:
                       _returnToFirst.worldInfo.ContainerWorldInfo.style.display = DisplayStyle.Flex;
                        break;
                }
                if (callback != null)
                    callback.Invoke();
                Invoke(() =>
                {
                    _returnToFirst.ButtonReturnToBack.RemoveFromClassList("Hide");
                    _returnToFirst.ButtonReturnToFirst.RemoveFromClassList("Hide");
                    _returnToFirst.step.ContainerStep.RemoveFromClassList("Hide");
                   _returnToFirst.worldInfo.ContainerWorldInfo.RemoveFromClassList("Hide");
                    _returnToFirst.ButtonReturnToBack.RemoveFromClassList("HideLeft");
                    _returnToFirst.ButtonReturnToFirst.RemoveFromClassList("HideLeft");
                    _returnToFirst.step.ContainerStep.RemoveFromClassList("HideLeft");
                   _returnToFirst.worldInfo.ContainerWorldInfo.RemoveFromClassList("HideLeft");
                    Invoke(() =>
                    {
                        _returnToFirst.step.ImgStep_Shadow.RemoveFromClassList("Shadow-Hide");
                        _returnToFirst.worldInfo.ImgWorldInfo_Shadow.RemoveFromClassList("Shadow-Hide");
                    }, .5f);
                }, 1f);
            }, .75f);
        }
    }
}
