using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Tilemaps;

public class MyPlayerContoller : PlayerController
{
    PortalController portal;
    GameObject Ladder;
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
    #region Ability
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
    #endregion
    protected override void Init()
    {
        base.Init();
        #region KeyDownBinding
        _keyDownHandler.Add(PlayerKeyInput.LeftArrow, () =>
        {
            if (HasState(PlayerState.Walk))
                return;
            AddState(PlayerState.Walk);
            Dir = MoveDirection.LEFT;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveStart.CreateC_MoveStart(builder, Dir, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveStart);
            Manager.Network.Send(pkt);
        });
        _keyDownHandler.Add(PlayerKeyInput.RightArrow, () =>
        {
            if (HasState(PlayerState.Walk))
                return;
            AddState(PlayerState.Walk);
            Dir = MoveDirection.RIGHT;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveStart.CreateC_MoveStart(builder, Dir, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveStart);
            Manager.Network.Send(pkt);
        });
        _keyDownHandler.Add(PlayerKeyInput.UpArrow, () =>
        {
            if (portal != null)
            {
                FlatBufferBuilder builder = new FlatBufferBuilder(50);

                var data = C_Portal.CreateC_Portal(builder, portal.PortalId);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_Portal);
                Manager.Network.Send(pkt);
            }
            else if (Ladder != null)
            {
                transform.position = new Vector3(Ladder.transform.position.x, transform.position.y);
                AddState(PlayerState.Ladder);
                Dir = MoveDirection.UP;

                FlatBufferBuilder builder = new FlatBufferBuilder(50);
                var data = C_LadderUpStart.CreateC_LadderUpStart(builder, transform.position.x, transform.position.y);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_LadderUpStart);
                Manager.Network.Send(pkt);
            }
        });
        _keyDownHandler.Add(PlayerKeyInput.DownArrow, () =>
        {
            if (Ladder != null)
            {
                transform.position = new Vector3(Ladder.transform.position.x, transform.position.y);
                AddState(PlayerState.Ladder);
                Dir = MoveDirection.DOWN;

                FlatBufferBuilder builder = new FlatBufferBuilder(50);
                var data = C_LadderDownStart.CreateC_LadderDownStart(builder, transform.position.x, transform.position.y);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_LadderDownStart);
                Manager.Network.Send(pkt);
            }
            else if (boxCollider.IsTouchingLayers(LayerMask.GetMask("Floor", "FloorBase", "Stair")))
            {
                if (HasState(PlayerState.ProneStab))
                    return;
                AddState(PlayerState.ProneStab);
                FlatBufferBuilder builder = new FlatBufferBuilder(50);

                C_ProneStabStart.StartC_ProneStabStart(builder);
                var data = C_ProneStabStart.EndC_ProneStabStart(builder);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_ProneStabStart);
                Manager.Network.Send(pkt);
            }
        });
        _keyDownHandler.Add(PlayerKeyInput.A, () =>
        {
            if (HasState(PlayerState.Attack))
                return;

            Attack();
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_Attack.CreateC_Attack(builder, (AttackEnum)tanjiro_Attack);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_Attack);
            Manager.Network.Send(pkt);
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
                Manager.Network.Send(pkt);
            }
        });
        #endregion
        #region KeyUpBinding
        _keyUpHandler.Add(PlayerKeyInput.LeftArrow, () =>
        {
            if (HasState(PlayerState.Walk) == false)
                return;
            RemoveState(PlayerState.Walk);
            Dir = MoveDirection.NONE;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveEnd.CreateC_MoveEnd(builder, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveEnd);
            Manager.Network.Send(pkt);
        });
        _keyUpHandler.Add(PlayerKeyInput.RightArrow, () =>
        {
            if (HasState(PlayerState.Walk) == false)
                return;
            RemoveState(PlayerState.Walk);
            Dir = MoveDirection.NONE;
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_MoveEnd.CreateC_MoveEnd(builder, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_MoveEnd);
            Manager.Network.Send(pkt);
        });
        _keyUpHandler.Add(PlayerKeyInput.UpArrow, () =>
        {
            if (HasState(PlayerState.Ladder) == false)
                return;
            Dir = MoveDirection.NONE;

            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_LadderUpEnd.CreateC_LadderUpEnd(builder, transform.position.x, transform.position.y);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_LadderUpEnd);
            Manager.Network.Send(pkt);
        });
        _keyUpHandler.Add(PlayerKeyInput.DownArrow, () =>
        {
            if (HasState(PlayerState.Ladder))
            {
                Dir = MoveDirection.NONE;

                FlatBufferBuilder builder = new FlatBufferBuilder(50);
                var data = C_LadderDownEnd.CreateC_LadderDownEnd(builder, transform.position.x, transform.position.y);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_LadderDownEnd);
                Manager.Network.Send(pkt);
            }
            else if (HasState(PlayerState.ProneStab))
            {
                RemoveState(PlayerState.ProneStab);
                FlatBufferBuilder builder = new FlatBufferBuilder(50);
                C_ProneStabEnd.StartC_ProneStabEnd(builder);
                var data = C_ProneStabEnd.EndC_ProneStabEnd(builder);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_ProneStabEnd);
                Manager.Network.Send(pkt);
            }
        });
        #endregion
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        base.TriggerEnter2D(collision);
        if (collision.gameObject.HasLayer("Portal"))
            portal = collision.gameObject.GetComponent<PortalController>();
        if (collision.gameObject.HasLayer("Ladder"))
            Ladder = collision.gameObject;
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        base.TriggerExit2D(collision);
        if (collision.gameObject.HasLayer("Portal"))
            portal = null;
        if (collision.gameObject.HasLayer("Portal"))
            Ladder = null;
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
