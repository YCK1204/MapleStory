using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;

public class MyPlayerContoller : PlayerController
{
    PortalController portal;
    enum PlayerKeyInput
    {
        None,
        LeftArrow,
        RightArrow,
        DownArrow,
        UpArrow,
        Space,
        A,
    }
    Dictionary<PlayerKeyInput, Action> _keyDownHandler = new Dictionary<PlayerKeyInput, Action>();
    Dictionary<PlayerKeyInput, Action> _keyUpHandler = new Dictionary<PlayerKeyInput, Action>();

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
        #region KeyDownBinding
        _keyDownHandler.Add(PlayerKeyInput.LeftArrow, () =>
        {
            Dir = MoveDirection.LEFT;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveStart.CreateC_MoveStart(builder, Dir, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveStart);
            //Manager.Network.Send(pkt);
        });
        _keyDownHandler.Add(PlayerKeyInput.RightArrow, () =>
        {
            Dir = MoveDirection.RIGHT;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveStart.CreateC_MoveStart(builder, Dir, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveStart);
            //Manager.Network.Send(pkt);
        });
        _keyDownHandler.Add(PlayerKeyInput.UpArrow, () =>
        {
            // 나중에 사다리, 로프도 추가

            if (portal == null)
                return;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);

            var data = C_Portal.CreateC_Portal(builder, portal.PortalId);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_Portal);
            //Manager.Network.Send(pkt);
        });
        _keyDownHandler.Add(PlayerKeyInput.A, () =>
        {
            if (State == PlayerState.Attack)
                return;
            tanjiro_Attack = (Tanjiro_Attack)(AttackCount++ % 4);
            State = PlayerState.Attack;
            _coAttack = StartCoroutine(CoAttack(.5f));

            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveStart.CreateC_MoveStart(builder, Dir, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveStart);
            //Manager.Network.Send(pkt);
        });
        _keyDownHandler.Add(PlayerKeyInput.Space, () =>
        {
            if (CanJump())
            {
                Jump();
                FlatBufferBuilder builder = new FlatBufferBuilder(50);
                C_Jump.StartC_Jump(builder);
                var data = C_Jump.EndC_Jump(builder);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_Jump);
                //Manager.Network.Send(pkt);
            }
        });
        _keyDownHandler.Add(PlayerKeyInput.DownArrow, () =>
        {
            State = PlayerState.ProneStab;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);

            C_ProneStabStart.StartC_ProneStabStart(builder);
            var data = C_ProneStabStart.EndC_ProneStabStart(builder);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_ProneStabStart);
            //Manager.Network.Send(pkt);
        });
        #endregion
        #region KeyUpBinding
        _keyUpHandler.Add(PlayerKeyInput.LeftArrow, () =>
        {
            Dir = MoveDirection.NONE;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveEnd.CreateC_MoveEnd(builder, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveEnd);
            //Manager.Network.Send(pkt);
        });
        _keyUpHandler.Add(PlayerKeyInput.RightArrow, () =>
        {
            Dir = MoveDirection.NONE;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveEnd.CreateC_MoveEnd(builder, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveEnd);
            //Manager.Network.Send(pkt);
        });
        _keyUpHandler.Add(PlayerKeyInput.DownArrow, () =>
        {
            State = PlayerState.Stand01;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            C_ProneStabEnd.StartC_ProneStabEnd(builder);
            var data = C_ProneStabEnd.EndC_ProneStabEnd(builder);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_ProneStabEnd);
            //Manager.Network.Send(pkt);
        });
        #endregion
        //var cc = Camera.main.gameObject.GetComponent<CameraController>();
        //cc.Target = gameObject;
        //cc.Init();
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.tag == "Portal")
            portal = collision.gameObject.GetComponent<PortalController>();
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.tag == "Portal")
            portal = null;
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
    private void HandleInput()
    {
        Action keyDownAction = null;
        Action keyUpAction = null;

        if (Input.anyKeyDown)
        {
            if (Input.GetKeyDown(KeyCode.LeftArrow))
                keyDownAction = _keyDownHandler[PlayerKeyInput.LeftArrow];
            else if (Input.GetKeyDown(KeyCode.RightArrow))
                keyDownAction = _keyDownHandler[PlayerKeyInput.RightArrow];
            else if (Input.GetKeyDown(KeyCode.Space))
                keyDownAction = _keyDownHandler[PlayerKeyInput.Space];
            else if (Input.GetKeyDown(KeyCode.DownArrow))
                keyDownAction = _keyDownHandler[PlayerKeyInput.DownArrow];
            else if (Input.GetKeyDown(KeyCode.UpArrow))
                keyDownAction = _keyDownHandler[PlayerKeyInput.UpArrow];
            else if (Input.GetKeyDown(KeyCode.A))
                keyDownAction = _keyDownHandler[PlayerKeyInput.A];
        }

        // AnyKeyUp
        {
            if (Input.GetKeyUp(KeyCode.LeftArrow))
                keyUpAction = _keyUpHandler[PlayerKeyInput.LeftArrow];
            else if (Input.GetKeyUp(KeyCode.RightArrow))
                keyUpAction = _keyUpHandler[PlayerKeyInput.RightArrow];
            else if (Input.GetKeyUp(KeyCode.DownArrow))
                keyUpAction = _keyUpHandler[PlayerKeyInput.DownArrow];
        }

        if (keyDownAction != null)
            keyDownAction.Invoke();
        if (keyUpAction != null)
            keyUpAction.Invoke();
    }
}
