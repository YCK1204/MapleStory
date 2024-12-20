using Google.FlatBuffers;
using System;
using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;

public partial class UIPrevInGameController : UIBaseController
{
    UINoticeController notice;
    VisualElement MainBG;

    enum BGState
    {
        Login,
        WorldSelect,
        CharacterSelect,
        CreateChacracter,
    }
    PopupState NoticeState
    {
        get
        {
            return notice.State;
        }
        set
        {
            notice.State = value;
        }
    }
    BGState _bgState;
    BGState BackgroundState
    {
        get { return _bgState; }
        set
        {
            _bgState = value;
            MainBG.ClearClassList();
            MainBG.AddToClassList("MainBG-Base");
            MainBG.AddToClassList($"MainBG-{value.ToString()}");
            CurAudioClip = ScrollUpAudio;
        }
    }
    protected override void Init()
    {
        base.Init();

        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);
        MainBG = _imgs["MainBG"];
        #region Login
        {
            Id = _textFields["Id"];
            Password = _textFields["Password"];
        }
        {
            _buttons["SignIn"].RegisterCallback<ClickEvent>(ClickSignInBtn);
            _buttons["SignUp"].RegisterCallback<ClickEvent>(ClickSignUpBtn);
            _buttons["GameExit"].RegisterCallback<ClickEvent>(ClickGameExitBtn);
        }
        #endregion'
        #region WorldSelect
        _worldBoard.Container = _containers["WorldBoard"];
        _worldBoard.WbScrollView = _scrollViews["WorldBoard"];

        _channelSelect.Container = _containers["ChannelSelect"];
        _channelSelect.ChannelScroll = _imgs["ChannelScroll"];
        _channelSelect.ChannelSelectMain = _imgs["ChannelSelectMain"];
        _channelSelect.ServerTitle = _imgs["ServerTitle"];
        _channelSelect.EnterSelectedChannel = _buttons["EnterSelectedChannel"];
        _channelSelect.CsScrollView = _scrollViews["ChannelSelect"];
        _channelSelect.EnterSelectedChannel.RegisterCallback<ClickEvent>(EnterChannel);
        InitializeServer();
        #endregion
    }
    public void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject
    {
        #region Login
        if (pkt is SC_SignUp signUp)
        {
            switch (signUp.Ok)
            {
                case SignUpError.SUCCESS: // ���� ����â ��ȯ
                    BackgroundState = BGState.WorldSelect;
                    break;
                case SignUpError.OVERLAPPED_USERID: // �̹� �ִ� ���̵� �Ǵ� �α��ε� ���̵�
                    NoticeState = PopupState.LoggedIn;
                    break;
                case SignUpError.UNKNOWN: // ������ ���� �߻�
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        else if (pkt is SC_SignIn signIn)
        {
            switch (signIn.Ok)
            {
                case SignInError.SUCCESS: // ���� ����â ��ȯ
                    BackgroundState = BGState.WorldSelect;
                    break;
                case SignInError.INVALID_ID: // �߸��� ���̵�� �α��� �Ǵ� ���� ���̵�
                    NoticeState = PopupState.InvalidId;
                    break;
                case SignInError.INVALID_PW: // �߸��� ���
                    NoticeState = PopupState.InvalidPassword;
                    break;
                case SignInError.UNKNOWN: // ������ ���� �߻�
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        #endregion
        #region WorldSelect
        else if (pkt is SC_ChannelInfo channelInfo) // ���� Ŭ�� ���� �� ��ȯ ��Ŷ
        {
            const int totalChannelCount = 20;
            ServerStruct server = _servers[channelInfo.ServerId];
            int channelCount = server.Channels.Length;

            _channelSelect.CsScrollView.Clear();
            for (int i = 0; i < channelCount; i++)
            {
                ChannelInfo channel = (ChannelInfo)channelInfo.Channels(i);
                //  button(ä��)
                //      guageFrame(ä�� ȥ�⵵ ��)
                //          guageContainer(ȥ�⵵ �� �ڵ鷯)
                //              guage(ȥ�⵵ �� �̹���)
                // ȥ�⵵ �ٸ� guageContainer�� width�� �ڵ�
                Button button = CreateButton(name: $"Channel_{i + 1}");
                VisualElement guageFrame = new VisualElement();
                VisualElement guageContainer = new VisualElement();
                VisualElement guage = new VisualElement();
                button.RegisterCallback<ClickEvent>(OnClickedChannel);
                button.Add(guageFrame);
                guageFrame.Add(guageContainer);
                guageContainer.Add(guage);

                button.AddToClassList($"Channel-Base");
                button.AddToClassList($"Channel-{i + 1}");

                string name = "ChannelGuage";
                guageFrame.AddToClassList($"{name}-Frame");
                guageContainer.AddToClassList($"{name}-Container");
                guage.AddToClassList($"{name}-Guage");

                guageContainer.style.width = 100 * (channel.UserCount / server.max_user_count);
                _channelSelect.ServerTitle.ClearClassList();
                _channelSelect.ServerTitle.AddToClassList($"ServerTitle-{server.Name}");
                _channelSelect.CsScrollView.Add(button);
            }
            for (int i = 0; i < totalChannelCount - channelCount; i++)
            {
                VisualElement temp = new VisualElement();
                temp.AddToClassList("Button-Base");
                temp.AddToClassList($"Channel-Base");
                temp.AddToClassList($"Channel-Empty");
                _channelSelect.CsScrollView.Add(temp);
            }
            StartImgAnimation(_channelSelect.ChannelScroll, 0, () => {
                CurAudioClip = RollDownAudio;
                Invoke(() => {
                    _channelSelect.ChannelSelectMain.RemoveFromClassList("ChannelSelect-Hide");
                }, .5f);
            });
        }
        else if (pkt is SC_EnterChannel enterChannel) // ä�� Ŭ�� ���� �� ��ȯ ��Ŷ
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
                    NoticeState = UINoticeController.PopupState.Unknown;
                    break;
            }
        }
        #endregion
    }
}
