using Google.FlatBuffers;
using System;
using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;

public partial class UIPrevInGameController : UIBaseController
{
    UINoticeController notice;
    UIReturnToFirstController returnToFirst;
    VisualElement MainBG;

    public enum BGState
    {
        Login,
        WorldSelect,
        CharacterSelect,
        CreateCharacter,
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
            returnToFirst.MoveImgs(value, () =>
            {
                MainBG.ClearClassList();
                MainBG.AddToClassList("MainBG-Base");
                MainBG.AddToClassList($"MainBG-{value.ToString()}");
                CurAudioClip = ScrollUpAudio;
            });
        }
    }
    protected override void Init()
    {
        base.Init();

        notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);
        returnToFirst = Util.FindChild<UIReturnToFirstController>(Manager.Scene.CurScene.UIControllers.transform);
        returnToFirst.ReturnToBack.RegisterCallback<ClickEvent>((e) => {
            --BackgroundState;
        });
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
        #region CharacterSelect
        SelectCharacter = _buttons["SelectCharacter"];
        CreateCharacter = _buttons["CreateCharacter"];
        DeleteCharacter = _buttons["DeleteCharacter"];

        SelectCharacter.RegisterCallback<ClickEvent>(HandleSelectChar);
        CreateCharacter.RegisterCallback<ClickEvent>(HandleCreateChar);
        DeleteCharacter.RegisterCallback<ClickEvent>(HandleDeleteChar);
        #endregion
        #region CreateCharacter
        _charStatus.NameCheck = _buttons["NameCheck"];
        _charStatus.Container = _imgs["CharacterStatus"];
        _charStatus.CharacterName = _textFields["CharacterName"];

        _charStatus.Main.Container = _containers["Main"];
        _charStatus.Main.DiceBtn = _buttons["Dice"];
        _charStatus.Main.DiceImg = _imgs["Dice"];
        _charStatus.Main.TextAbilityGroup = _groupBoxes["Ability"];
        _charStatus.Main.STR = _labels["STR"];
        _charStatus.Main.DEX = _labels["DEX"];
        _charStatus.Main.INT = _labels["INT"];
        _charStatus.Main.LUK = _labels["LUK"];

        CharacterImg = _imgs["Character"];
        CharacterList = _scrollViews["CharacterList"];
        CharacterInfo = _imgs["CharacterInfo"];
        CharacterCreate = _buttons["CharacterCreate"];


        CharacterCreate.RegisterCallback<ClickEvent>(FormCharacter);
        _charStatus.NameCheck.RegisterCallback<ClickEvent>(CheckName);
        _charStatus.Main.DiceBtn.RegisterCallback<ClickEvent>(GenerateAbilities);
        _charStatus.Main.DiceBtn.UnregisterCallback<MouseOverEvent>(OnMouseOverPlay);

        GenerateAbilities();
        InitializeCharList();
        #endregion
    }
    public void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject
    {
        #region Login
        if (pkt is SC_SignUp signUp)
        {
            switch (signUp.Ok)
            {
                case SignUpError.SUCCESS: // 서버 선택창 전환
                    BackgroundState = BGState.WorldSelect;
                    break;
                case SignUpError.OVERLAPPED_USERID: // 이미 있는 아이디 또는 로그인된 아이디
                    NoticeState = PopupState.LoggedIn;
                    break;
                case SignUpError.UNKNOWN: // 레전드 오류 발생
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        else if (pkt is SC_SignIn signIn)
        {
            switch (signIn.Ok)
            {
                case SignInError.SUCCESS: // 서버 선택창 전환
                    BackgroundState = BGState.WorldSelect;
                    break;
                case SignInError.INVALID_ID: // 잘못된 아이디로 로그인 또는 없는 아이디
                    NoticeState = PopupState.InvalidId;
                    break;
                case SignInError.INVALID_PW: // 잘못된 비번
                    NoticeState = PopupState.InvalidPassword;
                    break;
                case SignInError.UNKNOWN: // 레전드 오류 발생
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        #endregion
        #region WorldSelect
        else if (pkt is SC_ChannelInfo channelInfo) // 서버 클릭 했을 때 반환 패킷
        {
            const int totalChannelCount = 20;
            ServerStruct server = _servers[channelInfo.ServerId];
            int channelCount = server.Channels.Length;

            _channelSelect.CsScrollView.Clear();
            for (int i = 0; i < channelCount; i++)
            {
                ChannelInfo channel = (ChannelInfo)channelInfo.Channels(i);
                //  button(채널)
                //      guageFrame(채널 혼잡도 바)
                //          guageContainer(혼잡도 바 핸들러)
                //              guage(혼잡도 바 이미지)
                // 혼잡도 바를 guageContainer의 width로 핸들
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
        else if (pkt is SC_EnterChannel enterChannel) // 채널 클릭 했을 때 반환 패킷
        {
            switch (enterChannel.Ok)
            {
                case EnterChannelError.SUCCESS:
                    BackgroundState = BGState.CharacterSelect;
                    break;
                case EnterChannelError.FULL:
                    NoticeState = PopupState.Unknown;
                    break;
                case EnterChannelError.UNKNOWN:
                    NoticeState = UINoticeController.PopupState.Unknown;
                    break;
            }
        }
        #endregion
        #region CharacterSelect
        else if (pkt is SC_CreateCharacter createChar)
        {
            switch (createChar.Ok)
            {
                case CreateCharacterError.SUCCESS:
                    // 성공 시 캐릭터 생성 창으로 이동
                    break;
                case CreateCharacterError.FULL: // 에셋이 없어서 일단 unknown으로 대체 추후 full은 클라에서 disable 버튼으로 막을거임
                case CreateCharacterError.UNKNOWN:
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        #endregion
        #region CreateCharacter

        #endregion
    }
}
