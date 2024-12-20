using UnityEngine;

public class PrevInGameScene : BaseScene
{
    UIPrevInGameController prevInGameController;
    public override void OnRecvPacket<T>(T pkt)
    {
        prevInGameController.OnRecvPacket(pkt);
    }
    protected override void Init()
    {
        base.Init();
        prevInGameController = Util.FindChild<UIPrevInGameController>(UIControllers.transform);
    }
}
