using Google.FlatBuffers;
using System;
using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;
using System.Reflection;

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
    public BGState BackgroundState
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

        Invoke(() =>
        {
            notice = Util.FindChild<UINoticeController>(Manager.Scene.CurScene.UIControllers.transform);
            returnToFirst = Util.FindChild<UIReturnToFirstController>(Manager.Scene.CurScene.UIControllers.transform);
            returnToFirst._returnToFirst.ButtonReturnToBack.RegisterCallback<ClickEvent>((e) =>
            {
                --BackgroundState;
            });
            MainBG = _root.Q("Img-MainBG");
            #region Login
            AssignElements(login);
            login.ButtonSignIn.RegisterCallback<ClickEvent>(ClickSignInBtn);
            login.ButtonSignUp.RegisterCallback<ClickEvent>(ClickSignUpBtn);
            login.ButtonGameExit.RegisterCallback<ClickEvent>(ClickGameExitBtn);
            #endregion
            #region WorldSelect
            AssignElements(worldSelect);
            worldSelect.channelSelect.ButtonEnterSelectedChannel.RegisterCallback<ClickEvent>(EnterChannel);
            InitializeServer();
            #endregion
            #region CharacterSelect
            AssignElements(characterSelect.navi);
            characterSelect.characterList.ContainerCharacterList = _root.Q("Container-CharacterList");
            for (int i = 1; i <= characterCount; i++)
            {
                Button button = _root.Q<Button>($"Button-Character{i}");

                button.UnregisterCallback<MouseOverEvent>(OnMouseOverPlay);
                characterButtons.Add(button);
                characterSelect.characterList.characterPanel[i - 1] = new CharacterStatusPanel();
                AssignElements(characterSelect.characterList.characterPanel[i - 1], button);
            }

            characterSelect.navi.ButtonSelectCharacter.RegisterCallback<ClickEvent>(HandleSelectChar);
            characterSelect.navi.ButtonCreateCharacter.RegisterCallback<ClickEvent>(HandleCreateChar);
            characterSelect.navi.ButtonDeleteCharacter.RegisterCallback<ClickEvent>(HandleDeleteChar);
            #endregion
            #region CreateCharacter
            AssignElements(createCharacter);

            createCharacter.ButtonCharacterCreate.RegisterCallback<ClickEvent>(FormCharacter);
            createCharacter.characterAbility.ButtonNameCheck.RegisterCallback<ClickEvent>(CheckName);
            createCharacter.characterAbility.ButtonDice.RegisterCallback<ClickEvent>(GenerateAbilities);
            createCharacter.characterAbility.ButtonDice.UnregisterCallback<MouseOverEvent>(OnMouseOverPlay);

            GenerateAbilities();
            InitializeCharList();
            #endregion
        });
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

            worldSelect.channelSelect.ScrollViewChannelSelect.Clear();
            for (int i = 0; i < channelCount; i++)
            {
                ChannelInfo channel = (ChannelInfo)channelInfo.Channels(i);

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
                worldSelect.channelSelect.ImgServerTitle.ClearClassList();
                worldSelect.channelSelect.ImgServerTitle.AddToClassList($"ServerTitle-{server.Name}");
                worldSelect.channelSelect.ScrollViewChannelSelect.Add(button);
            }
            for (int i = 0; i < totalChannelCount - channelCount; i++)
            {
                VisualElement temp = new VisualElement();
                temp.AddToClassList("Button-Base");
                temp.AddToClassList($"Channel-Base");
                temp.AddToClassList($"Channel-Empty");
                worldSelect.channelSelect.ScrollViewChannelSelect.Add(temp);
            }
            StartImgAnimation(worldSelect.channelSelect.ImgChannelScroll, 0, () =>
            {
                CurAudioClip = RollDownAudio;
                Invoke(() =>
                {
                    worldSelect.channelSelect.ImgChannelSelectMain.RemoveFromClassList("ChannelSelect-Hide");
                }, .5f);
            });
        }
        else if (pkt is SC_EnterChannel enterChannel) // 채널 클릭 했을 때 반환 패킷
        {
            switch (enterChannel.Ok)
            {
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
        else if (pkt is SC_CharacterList characterList)
        {
            switch (characterList.Ok)
            {
                case CharacterListError.SUCCESS:
                    var scene = Manager.Scene.CurScene as PrevInGameScene;
                    var pc = scene.prevInGameController;
                    pc.BackgroundState = UIPrevInGameController.BGState.CharacterSelect;
                    break;
                case CharacterListError.UNKNOWN:
                    break;
            }
        }
        #endregion
        #region CreateCharacter

        #endregion
    }
}
