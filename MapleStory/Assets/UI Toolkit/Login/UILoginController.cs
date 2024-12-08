using Google.FlatBuffers;
using System;
using UnityEngine;
using UnityEngine.UIElements;

public class UILoginController : UIBaseController
{
    TextField Id;
    TextField Password;
    readonly int LengthRangeStart = 4;
    readonly int LengthRangeEnd = 20;
    Popup _popup = new Popup();

    enum PopupState : ushort
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
        /* NORMAL */
    }
    struct Popup
    {
        public VisualElement Container;
        public VisualElement BG;
        public VisualElement Loading;
        public VisualElement Text;
        public Button Ok;
        public Button Yes;
        public Button No;
    }
    PopupState _state;
    PopupState State
    {
        get { return _state; }
        set
        {
            _state = value;

            // 상황별 팝업 문자 이미지 전환
            {
                _popup.BG.ClearClassList();
                _popup.Text.ClearClassList();
                string bgState = "Normal";
                string textState = _state.ToString();
                if (_state <= PopupState.InputPattern)
                    bgState = "Crash";
                _popup.BG.AddToClassList($"PopupBG-{bgState}");
                _popup.Text.AddToClassList($"PopupText-{textState}");
                _popup.Container.RemoveFromClassList("Popup-Hide");
            }
            // 버튼 상황별 숨기기
            {
                _popup.Ok.style.visibility = Visibility.Hidden;
                _popup.Yes.style.visibility = Visibility.Hidden;
                _popup.No.style.visibility = Visibility.Hidden;

                if (_state < PopupState.GameExit)
                {
                    _popup.Ok.style.visibility = Visibility.Visible;
                }
                else if (_state == PopupState.GameExit)
                {
                    _popup.Yes.style.visibility = Visibility.Visible;
                    _popup.No.style.visibility = Visibility.Visible;
                }
            }
        }
    }

    protected override void Init()
    {
        base.Init();

        {
            Id = _textFields["Id"];
            Password = _textFields["Password"];
        }
        {
            _buttons["SignIn"].RegisterCallback<ClickEvent>(ClickSignInBtn);
            _buttons["SignUp"].RegisterCallback<ClickEvent>(ClickSignUpBtn);
            _buttons["GameExit"].RegisterCallback<ClickEvent>(ClickGameExitBtn);
        }
        {
            _popup.Container = _containers["Popup"];
            _popup.BG = _imgs["PopupBackground"];
            _popup.Loading = _imgs["Loading"];
            _popup.Text = _imgs["PopupText"];
            _popup.Ok = _buttons["PopupOk"];
            _popup.Yes = _buttons["PopupYes"];
            _popup.No = _buttons["PopupNo"];
            _popup.Ok.RegisterCallback<ClickEvent>((e) => { _popup.Container.AddToClassList("Popup-Hide"); });
            _popup.Yes.RegisterCallback<ClickEvent>((e) =>
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
            _popup.No.RegisterCallback<ClickEvent>((e) => { _popup.Container.AddToClassList("Popup-Hide"); });
        }
    }
    private void BaseSignBtn<T>(PacketType type, Func<FlatBufferBuilder, StringOffset, StringOffset, Offset<T>> create) where T : struct, IFlatbufferObject
    {
        string idText = Id.text;
        string passwordText = Password.text;

        if (idText.isAlNum() == false || passwordText.isAlNum() == false)
        {
            State = PopupState.InputPattern;
            return;
        }
        if (idText.IsInLengthRange(LengthRangeStart, LengthRangeEnd) == false || passwordText.IsInLengthRange(LengthRangeStart, LengthRangeEnd) == false)
        {
            State = PopupState.Length;
            return;
        }

        FlatBufferBuilder builder = new FlatBufferBuilder(100);

        var id = builder.CreateString(idText);
        var password = builder.CreateString(passwordText);
        var data = create(builder, id, password);
        var pkt = Manager.Packet.CreatePacket(data, builder, type);
        Manager.Network.Send(pkt);
    }
    private void ClickSignInBtn(ClickEvent e)
    {
        BaseSignBtn<C_SignIn>(PacketType.C_SignIn, C_SignIn.CreateC_SignIn);
    }
    private void ClickSignUpBtn(ClickEvent e)
    {
        BaseSignBtn<C_SignUp>(PacketType.C_SignUp, C_SignUp.CreateC_SignUp);
    }
    private void ClickGameExitBtn(ClickEvent e)
    {
        State = PopupState.GameExit;
    }
    public void OnRecvPacket<T>(T pkt)  where T : struct, IFlatbufferObject
    {
        if (pkt is SC_SignUp signUp)
        {
            switch (signUp.Ok)
            {
                case SignUpError.SUCCESS:
                    State = PopupState.CreatedId;
                    break;
                case SignUpError.OVERLAPPED_USERID:
                    State = PopupState.RegisteredId;
                    break;
                case SignUpError.UNKNOWN:
                    State = PopupState.Unknown;
                    break;
            }
        }
        else if (pkt is SC_SignIn signIn)
        {
            switch (signIn.Ok)
            {
                case SignInError.SUCCESS:
                    State = PopupState.CreatedId;
                    break;
                case SignInError.NOT_FOUND:
                    State = PopupState.Id;
                    break;
                case SignInError.INVALID_PW:
                    State = PopupState.Password;
                    break;
                case SignInError.UNKNOWN:
                    State = PopupState.Unknown;
                    break;
            }
        }
    }
}
