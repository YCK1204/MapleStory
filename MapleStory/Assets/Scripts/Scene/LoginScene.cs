using Google.FlatBuffers;
using UnityEngine;
using UnityEngine.Diagnostics;

public class LoginScene : BaseScene
{
    UILoginController uiLoginController;

    public override void OnRecvPacket<T>(T pkt)
    {
        uiLoginController.OnRecvPacket(pkt);
    }

    protected override void Init()
    {
        base.Init();

        uiLoginController = Util.FindChild<UILoginController>(UIControllers.transform);
    }
}
