using UnityEngine;

public class WorldSelectScene : BaseScene
{
    UIWorldSelectController worldSelectController;
    protected override void Init()
    {
        base.Init();
        PrevSceneName = "Login";
        worldSelectController = Util.FindChild<UIWorldSelectController>(UIControllers.transform);
    }
    public override void OnRecvPacket<T>(T pkt)
    {
        worldSelectController.OnRecvPacket(pkt);
    }
}
