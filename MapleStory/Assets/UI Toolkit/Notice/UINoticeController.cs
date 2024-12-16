using UnityEngine;
using UnityEngine.UIElements;

public class UINoticeController : UIBaseController
{
    Popup _popup = new Popup();

    public enum PopupState : ushort
    {
        /* CRASH */
        Password,
        Id,
        Length,
        Unknown,
        InputPattern,
        /* CRASH */

        /* NORMAL */
        CreatedId,
        RegisteredId,
        GameExit,
        GettingCharacter,
        ConnectingServer,
        BackToFirst,
        /* NORMAL */
    }
    struct Popup
    {
        public VisualElement Container;
        public VisualElement Loading;
        public Button Ok;
        public Button Exit;
        public Button Yes;
        public Button No;
    }
    PopupState _state;
    public PopupState State
    {
        get { return _state; }
        set
        {
            _state = value;

            // 버튼 상황별 숨기기
            {
                _popup.Ok.style.visibility = Visibility.Hidden;
                _popup.Exit.style.visibility = Visibility.Hidden;
                _popup.No.style.visibility = Visibility.Hidden;
                _popup.Yes.style.visibility = Visibility.Hidden;

                if (_state < PopupState.GameExit)
                {
                    _popup.Ok.style.visibility = Visibility.Visible;
                }
                else if (_state == PopupState.GameExit)
                {
                    _popup.Exit.style.visibility = Visibility.Visible;
                    _popup.No.style.visibility = Visibility.Visible;
                }
                else if (_state == PopupState.BackToFirst)
                {
                    _popup.Yes.style.visibility = Visibility.Visible;
                    _popup.No.style.visibility = Visibility.Visible;
                }
            }

            // 상황별 팝업 문자 이미지 전환
            {
                string textState = _state.ToString();

                _popup.Container.ClearClassList();
                _popup.Container.AddToClassList("Popup-Show");
                _popup.Container.AddToClassList($"PopupText-{textState}");
                _popup.Container.RemoveFromClassList("Popup-Hide");
            }
        }
    }
    protected override void Init()
    {
        base.Init();
        {
            _popup.Container = _containers["Popup"];
            _popup.Loading = _imgs["Loading"];
            _popup.Ok = _buttons["Ok"];
            _popup.Exit = _buttons["GameExit"];
            _popup.No = _buttons["No"];
            _popup.Yes = _buttons["Yes"];
            _popup.Ok.RegisterCallback<ClickEvent>((e) =>
            {
                _popup.Container.AddToClassList("Popup-Hide");
            });
            _popup.Yes.RegisterCallback<ClickEvent>((e) =>
            {
                Manager.Scene.LoadScene("Login");
            });
            _popup.Exit.RegisterCallback<ClickEvent>((e) =>
            {
                if (_state == PopupState.GameExit)
                {
#if UNITY_EDITOR
                    UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit(); // 어플리케이션 종료
#endif
                }
            });
            _popup.No.RegisterCallback<ClickEvent>((e) =>
            {
                _popup.Container.AddToClassList("Popup-Hide");
            });
        }
    }
}
