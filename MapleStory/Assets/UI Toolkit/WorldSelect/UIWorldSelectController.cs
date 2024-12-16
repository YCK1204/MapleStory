using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.Xml.Schema;
using UnityEditor;

public class UIWorldSelectController : UIBaseController
{
    [SerializeField]
    TextAsset ServerText;

    [SerializeField]
    float WorldElementHeight = 50f;

    #region JsonStructs
    struct ChannelStruct
    {
        public int Id { get; set; }
    }
    struct ServerStruct
    {
        public byte Id { get; set; }
        public string Name { get; set; }
        public ushort MaxUserCount { get; set; }
        public ChannelStruct[] Channels { get; set; }
    }
    struct ServerList
    {
        public ServerStruct[] Servers { get; set; }
    }
    #endregion


    ListView _worldSelectListView;
    ScrollView _channelSelectScrollView;
    List<Button> _worldButtons = new List<Button>();

    Dictionary<byte, ServerStruct> _servers = new Dictionary<byte, ServerStruct>();

    VisualElement _containerChannelSelect;
    Button _curButton = null;
    VisualElement _serverTitle;
    UINoticeController notice;
    protected override void Init()
    {
        #region Init
        base.Init();
        _worldSelectListView = _listViews["WorldSelect"];
        _channelSelectScrollView = _scrollViews["ChannelSelect"];
        _containerChannelSelect = _containers["ChannelSelect"];
        _serverTitle = _imgs["ServerTitle"];
        _buttons["goToSelectedChannel"].RegisterCallback<ClickEvent>(EnterChannel);
        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);

        _containerChannelSelect.RegisterCallback<ClickEvent>((e) =>
        {
            if (e.target == _containerChannelSelect)
            {
                _containerChannelSelect.style.display = DisplayStyle.None;
                Debug.Log("?");
            }
        });

        Func<Button> makeItem = () =>
        {
            var button = new Button();

            button.AddToClassList("Button-Base");
            button.RegisterCallback<ClickEvent>(OnMouseClickPlay);
            button.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);

            return button;
        };
        Action<VisualElement, int> bindItem = (b, i) =>
        {
            Button button = b as Button;

            button.name = $"Button-Server_{i}";
            button.AddToClassList($"Button-{_servers[(byte)i].Name}");
            button.RegisterCallback<ClickEvent>(OnClickedServer);

            button.RegisterCallback<ClickEvent>((e) =>
            {
                int idx = button.name.IndexOf("_");
                int serverIdx = int.Parse(button.name.Substring(idx + 1));
            });
        };
        _worldSelectListView.makeItem = makeItem;
        _worldSelectListView.bindItem = bindItem;
        _worldSelectListView.itemsSource = _worldButtons;
        _worldSelectListView.selectionType = SelectionType.Multiple;
        _worldSelectListView.fixedItemHeight = WorldElementHeight;
        #endregion
        InitializeServer();
    }
    private void InitializeServer()
    {
        string serverJsonTxt = ServerText.ToString();

        ServerList servers = JsonConvert.DeserializeObject<ServerList>(serverJsonTxt);
        foreach (ServerStruct server in servers.Servers)
        {
            _servers.Add(server.Id, server);
            Button serverButton = _worldSelectListView.makeItem() as Button;
            _worldSelectListView.bindItem(serverButton, server.Id);
            _worldButtons.Add(serverButton);
        }
        _worldSelectListView.RefreshItems();
    }

    public void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject
    {
        if (pkt is SC_ChannelInfo channelInfo)
        {
            ServerStruct server = _servers[channelInfo.ServerId];
            int channelCount = server.Channels.Length;

            _channelSelectScrollView.Clear();
            string name = "Button-Channel";
            _serverTitle.ClearClassList();
            _serverTitle.AddToClassList($"ServerTitle-{server.Name}");
            for (int i = 0; i < channelCount; i++)
            {
                ChannelInfo channel = (ChannelInfo)channelInfo.Channels(i);
                var channelButton = new Button();
                var slider = new Slider();


                channelButton.name = $"{name}_{i}";
                channelButton.AddToClassList($"{name}");
                channelButton.AddToClassList($"{name}{i + 1}");
                channelButton.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
                channelButton.RegisterCallback<ClickEvent>(OnMouseClickPlay);
                channelButton.RegisterCallback<ClickEvent>(OnClickedChannel);
                channelButton.Add(slider);

                slider.AddToClassList("Slider-ChannelGuage");
                slider.fill = true;
                slider.value = (float)channel.UserCount / server.MaxUserCount;
                slider.lowValue = 0f;
                slider.highValue = 1f;

                _channelSelectScrollView.Add(channelButton);
            }
            _containerChannelSelect.style.display = DisplayStyle.Flex;
        }
        else if (pkt is SC_EnterChannel enterChannel)
        {
            switch (enterChannel.Ok)
            {
                case EnterChannelError.SUCCESS:
                    Debug.Log("Success");
                    break;
                case EnterChannelError.FULL:
                    Debug.Log("Full");
                    break;
                case EnterChannelError.UNKNOWN:
                    notice.State = UINoticeController.PopupState.Unknown;
                    break;
            }
        }
    }
    // 서버 클릭 시 서버 내 채널 정보 요청
    private void OnClickedServer(ClickEvent e)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(50);
        ServerStruct server = _servers[(byte)ExtractIdFromElementName(e)];

        var data = C_ChannelInfo.CreateC_ChannelInfo(builder, server.Id);
        var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_ChannelInfo);
        Manager.Network.Send(pkt);
    }
    // 채널 클릭 했을 때
    private void OnClickedChannel(ClickEvent e)
    {
        if (e.currentTarget as Button == null)
            return;

        if (_curButton == null)
        {
            _curButton = e.currentTarget as Button;
            return;
        }
        else if (_curButton != e.currentTarget as Button)
        {
            _curButton = e.currentTarget as Button;
            return;
        }
        EnterChannel(e);
    }
    private string GetServerNameFromServerTitle()
    {
        foreach (var str in _serverTitle.GetClasses())
        {
            if (str.StartsWith("ServerTitle-"))
                return (str.Substring(str.IndexOf('-') + 1));
        }
        return null;
    }

    private void EnterChannel(ClickEvent e)
    {
        if (_curButton == null)
            return;
        FlatBufferBuilder builder = new FlatBufferBuilder(50);
        byte channelIdx = (byte)ExtractIdFromElementName(_curButton.name);

        string serverName = GetServerNameFromServerTitle();

        foreach (var pair in _servers)
        {
            var server = pair.Value;
            if (server.Name == serverName)
            {
                var data = C_EnterChannel.CreateC_EnterChannel(builder, server.Id, channelIdx);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_EnterChannel);
                Manager.Network.Send(pkt);
            }
        }
    }
}
