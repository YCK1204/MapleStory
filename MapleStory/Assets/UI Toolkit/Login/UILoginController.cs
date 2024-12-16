using Google.FlatBuffers;
using System;
using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;

public class UILoginController : UIBaseController
{
    TextField Id;
    TextField Password;
    readonly int LengthRangeStart = 4;
    readonly int LengthRangeEnd = 20;
    UINoticeController notice;
    
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
            _buttons["Exit"].RegisterCallback<ClickEvent>(ClickGameExitBtn);
        }
        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);
    }
    private void BaseSignBtn<T>(PacketType type, Func<FlatBufferBuilder, StringOffset, StringOffset, Offset<T>> create) where T : struct, IFlatbufferObject
    {
        string idText = Id.text;
        string passwordText = Password.text;

        if (idText.isAlNum() == false || passwordText.isAlNum() == false)
        {
            notice.State = PopupState.InputPattern;
            return;
        }
        if (idText.IsInLengthRange(LengthRangeStart, LengthRangeEnd) == false || passwordText.IsInLengthRange(LengthRangeStart, LengthRangeEnd) == false)
        {
            notice.State = PopupState.Length;
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
        notice.State = PopupState.GameExit;
    }
    public void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject
    {
        if (pkt is SC_SignUp signUp)
        {
            switch (signUp.Ok)
            {
                case SignUpError.SUCCESS:
                    notice.State = PopupState.CreatedId;
                    break;
                case SignUpError.OVERLAPPED_USERID:
                    notice.State = PopupState.RegisteredId;
                    break;
                case SignUpError.UNKNOWN:
                    notice.State = PopupState.Unknown;
                    break;
            }
        }
        else if (pkt is SC_SignIn signIn)
        {
            switch (signIn.Ok)
            {
                case SignInError.SUCCESS:
                    Manager.Scene.LoadScene("WorldSelect");
                    break;
                case SignInError.NOT_FOUND:
                    notice.State = PopupState.Id;
                    break;
                case SignInError.INVALID_PW:
                    notice.State = PopupState.Password;
                    break;
                case SignInError.UNKNOWN:
                    notice.State = PopupState.Unknown;
                    break;
            }
        }
    }
}
