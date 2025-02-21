using Google.FlatBuffers;
using UnityEngine;

public class MyPlayerContoller : PlayerController
{
    class Ability
    {
        public int STR;
        public int DEX;
        public int INT;
        public int LUK;
    }
    Ability _ability = new Ability();
    public int STR { get { return _ability.STR; } set { _ability.STR = value; } }
    public int DEX { get { return _ability.DEX; } set { _ability.DEX = value; } }
    public int INT { get { return _ability.INT; } set { _ability.INT = value; } }
    public int LUK { get { return _ability.LUK; } set { _ability.LUK = value; } }
    public int EXP { get; set; }
    public int HP { get; set; }
    public int MP { get; set; }
    protected override void Init()
    {
        base.Init();
        var cc = Camera.main.gameObject.GetComponent<CameraController>();
        cc.Target = gameObject;
        cc.Init();
    }
    protected override void UpdateController()
    {
        base.UpdateController();
    }
    private void Update()
    {
        HandleInput();
        UpdateController();
    }
    FlatBufferBuilder builder = new FlatBufferBuilder(50);
    private void HandleInput()
    {
        bool up = false;
        float x = transform.position.x;
        float y = transform.position.y;

        if (Input.anyKeyDown)
        {
            if (Input.GetKeyDown(KeyCode.LeftArrow))
                Dir = MoveDirection.LEFT;
            else if (Input.GetKeyDown(KeyCode.RightArrow))
                Dir = MoveDirection.RIGHT;
            else
                return;
            builder.Clear();
            var data = C_MoveStart.CreateC_MoveStart(builder, Dir, x, y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveStart);
            Manager.Network.Send(pkt);
        }
        if ((up |= Input.GetKeyUp(KeyCode.LeftArrow)) || (up |= Input.GetKeyUp(KeyCode.RightArrow)))
            Dir = MoveDirection.NONE;
        if (up)
        {
            builder.Clear();
            var data = C_MoveEnd.CreateC_MoveEnd(builder, x, y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveEnd);
            Manager.Network.Send(pkt);
        }
    }
}
