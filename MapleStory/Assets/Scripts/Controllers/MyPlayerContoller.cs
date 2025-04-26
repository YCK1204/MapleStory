using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using System.Threading;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.Tilemaps;

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
        Z,
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
    public int Money { get; set; }
    #endregion
    [SerializeField]
    float X = 4f;
    [SerializeField]
    float Y = 3f;
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
        });
        _keyDownHandler.Add(PlayerKeyInput.DownArrow, () =>
        {
            if (boxCollider.IsTouchingLayers(LayerMask.GetMask("Floor", "FloorBase", "Stair")))
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
            Vector2 center = (Vector2)transform.position + (Vector2.right * (transform.localScale.x > 0 ? -XX : XX)) + Vector2.up * YY; // 앞 방향 (오른쪽)으로 1만큼 이동한 위치
            Vector2 size = new Vector2(X, Y); // 가로 2, 세로 1 범위  

            Collider2D[] hits = Physics2D.OverlapBoxAll(center, size, 0f, LayerMask.GetMask("Enemy"));
            List<ulong> targets = new List<ulong>();
            foreach (var hit in hits)
            {
                var mc = hit.GetComponent<MonsterController>();
                if (mc.State == MonsterState.Hit)
                    return;
                mc.DestPosX = transform.position.x;
                mc.State = MonsterState.Hit;
                Invoke(() =>
                {
                    mc.State = MonsterState.Move;
                }, 1f);
                targets.Add(mc.ID);
            }
            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var vector = C_Attack.CreateTargetsVector(builder, targets.ToArray());
            var data = C_Attack.CreateC_Attack(builder, (AttackEnum)tanjiro_Attack, vector);
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
        _keyDownHandler.Add(PlayerKeyInput.Z, () =>
        {
            Vector2 size = new Vector2(2, 2);
            Collider2D[] items = Physics2D.OverlapBoxAll(transform.position, size, 0f, LayerMask.GetMask("Item"));
            GameObject go = null;
            foreach (var collision in items)
            {
                if (go == null)
                    go = collision.gameObject;
                else
                {
                    if ((gameObject.transform.position - go.transform.position).magnitude > (gameObject.transform.position - collision.transform.position).magnitude)
                        go = collision.gameObject;
                }
            }
            if (go != null)
            {
                var cc = go.GetComponent<CoinController>();
                FlatBufferBuilder builder = new FlatBufferBuilder(1);
                var data = C_CollectCoin.CreateC_CollectCoin(builder, cc.ID);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_CollectCoin);
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
        _keyUpHandler.Add(PlayerKeyInput.DownArrow, () =>
        {
            if (HasState(PlayerState.ProneStab))
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
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        base.TriggerExit2D(collision);
        if (collision.gameObject.HasLayer("Portal"))
            portal = null;
    }
    protected override void UpdateController()
    {
        base.UpdateController();
    }
    float PosNotiTick = 1f;
    float LastPosNotiTime = Time.deltaTime;
    private void Update()
    {
        HandleInput();
        UpdateController();
        if (HasState(PlayerState.Walk) && LastPosNotiTime + PosNotiTick < Time.deltaTime)
        {

        }
    }
    [SerializeField]
    float XX = 2.5f;
    [SerializeField]
    float YY = .5f;
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
            else if (Input.GetKeyDown(KeyCode.Z))
                keyDownAction = _keyDownHandler[PlayerKeyInput.Z];
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
