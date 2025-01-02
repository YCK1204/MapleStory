using Google.FlatBuffers;
using System;
using UnityEngine;
using static UINoticeController;
using UnityEngine.UIElements;

public partial class UIPrevInGameController : UIBaseController
{
    TextField Id;
    TextField Password;
    static readonly byte MaxLength = 20;
    private void HandleSign<T>(PacketType type, Func<FlatBufferBuilder, StringOffset, StringOffset, Offset<T>> create) where T : struct, IFlatbufferObject
    {
        string idText = Id.text;
        string passwordText = Password.text;

        if (idText.Length < 1 || passwordText.Length < 1)
            return;

        if (idText.isAlNum() == false || idText.Length > MaxLength)
        {
            notice.State = PopupState.InvalidId;
            return;
        }
        if (passwordText.isAlNum() == false || passwordText.Length > MaxLength)
        {
            notice.State = PopupState.InvalidPassword;
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
        HandleSign<C_SignIn>(PacketType.C_SignIn, C_SignIn.CreateC_SignIn);
    }
    private void ClickSignUpBtn(ClickEvent e)
    {
        HandleSign<C_SignUp>(PacketType.C_SignUp, C_SignUp.CreateC_SignUp);
    }
    private void ClickGameExitBtn(ClickEvent e)
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
                Application.Quit(); // 어플리케이션 종료
#endif
    }
}