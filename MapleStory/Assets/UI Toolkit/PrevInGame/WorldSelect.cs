using Google.FlatBuffers;
using Newtonsoft.Json;
using System.Collections.Generic;
using System;
using UnityEngine;
using UnityEngine.UIElements;
using System.Linq;

public partial class UIPrevInGameController : UIBaseController
{
    [SerializeField]
    TextAsset ServerInfoJson;
    [SerializeField]
    AudioClip ScrollUpAudio;
    [SerializeField]
    AudioClip RollDownAudio;
    [SerializeField]
    AudioClip ChannelSelectAudio;
    #region JsonStructs
    struct ChannelStruct
    {
        public int Id { get; set; }
    }
    struct ServerStruct
    {
        public byte Id { get; set; }
        public string Name { get; set; }
        public ushort max_user_count { get; set; }
        public ChannelStruct[] Channels { get; set; }
    }
    struct ServerList
    {
        public ServerStruct[] Servers { get; set; }
    }
    #endregion

    struct WorldBoard
    {
        public VisualElement Container;
        public ScrollView WbScrollView;
    }
    struct ChannelSelect
    {
        public VisualElement Container;
        public VisualElement ChannelScroll;
        public VisualElement ChannelSelectMain;
        public VisualElement ServerTitle;
        public Button EnterSelectedChannel;
        public ScrollView CsScrollView;
    }

    WorldBoard _worldBoard = new WorldBoard();
    ChannelSelect _channelSelect = new ChannelSelect();

    Dictionary<byte, ServerStruct> _servers = new Dictionary<byte, ServerStruct>();

    Button _selectedChannelBtn;
    Button SelectedChannelBtn
    {
        get { return _selectedChannelBtn; }
        set
        {
            VisualElement sign = null;
            if (_selectedChannelBtn != null)
                sign = _selectedChannelBtn.Q<VisualElement>("Channel-Sign");
            if (sign != null)
                _selectedChannelBtn.Remove(sign);
            sign = new VisualElement() { name = "Channel-Sign" };

            CurAudioClip = ChannelSelectAudio;
            _selectedChannelBtn = value;
            _selectedChannelBtn.Add(sign);
            sign.AddToClassList("Channel-Sign");
            StartImgAnimation(sign, 1);
        }
    }
    private void InitializeServer()
    {
        const int totalServerCnt = 20;
        string serverJsonTxt = ServerInfoJson.ToString();

        ServerList servers = JsonConvert.DeserializeObject<ServerList>(serverJsonTxt);
        foreach (ServerStruct server in servers.Servers) // Json ���Ͽ� �ִ� ������ ��ưȭ
        {
            _servers.Add(server.Id, server);
            Button serverButton = CreateButton();
            serverButton.RegisterCallback<ClickEvent>(OnClickedServer);
            serverButton.name = $"Button-Server_{server.Id}";
            serverButton.AddToClassList("Button-World");
            serverButton.AddToClassList($"Button-{server.Name}");
            _worldBoard.WbScrollView.Add(serverButton);
        }
        for (int i = 0; i < totalServerCnt - servers.Servers.Length; i++) // ���� ������ empty �̹��� ��ü
        {
            VisualElement tempWorld = new VisualElement();
            tempWorld.AddToClassList("Button-Base");
            tempWorld.AddToClassList("Button-World");
            tempWorld.AddToClassList($"Button-EmptyWorld");
            _worldBoard.WbScrollView.Add(tempWorld);
        }
    }
    // ���� Ŭ�� �� ���� �� ä�� ���� ��û
    private void OnClickedServer(ClickEvent e)
    {
        _channelSelect.ChannelSelectMain.AddToClassList("ChannelSelect-Hide");

        FlatBufferBuilder builder = new FlatBufferBuilder(50);
        ServerStruct server = _servers[(byte)ExtractIdFromElementName(e)];

        var data = C_ChannelInfo.CreateC_ChannelInfo(builder, server.Id);
        var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_ChannelInfo);
        Manager.Network.Send(pkt);
    }
    // ä�� Ŭ�� ���� ��
    private void OnClickedChannel(ClickEvent e)
    {
        if (e.currentTarget as Button == null)
            return;
        if (SelectedChannelBtn == null)
        {
            SelectedChannelBtn = e.currentTarget as Button;
            return;
        }
        else if (SelectedChannelBtn != e.currentTarget as Button)
        {
            SelectedChannelBtn = e.currentTarget as Button;
            return;
        }
        EnterChannel(e);
    }
    private string GetServerNameFromServerTitle()
    {
        foreach (var str in _channelSelect.ServerTitle.GetClasses())
        {
            if (str.StartsWith("ServerTitle-"))
                return (str.Substring(str.IndexOf('-') + 1));
        }
        return null;
    }
    private void EnterChannel(ClickEvent e)
    {
        if (SelectedChannelBtn == null)
        {
            NoticeState = UINoticeController.PopupState.SelectChannel;
            return;
        }
        FlatBufferBuilder builder = new FlatBufferBuilder(50);
        byte channelIdx = (byte)ExtractIdFromElementName(SelectedChannelBtn.name);

        string serverName = GetServerNameFromServerTitle();

        try
        {
            var server = _servers.First((s) => s.Value.Name == serverName);
            var data = C_EnterChannel.CreateC_EnterChannel(builder, server.Value.Id, channelIdx);
            var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_EnterChannel);
            Manager.Network.Send(pkt);
        }
        catch (Exception ex)
        {
            Debug.LogError(ex);
        }
    }
}