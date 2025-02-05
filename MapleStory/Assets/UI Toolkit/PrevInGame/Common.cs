using Google.FlatBuffers;
using System;
using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;
using System.Reflection;
using UnityEditor.Animations;
using System.Collections.Generic;
using Newtonsoft.Json.Linq;

public partial class UIPrevInGameController : UIBaseController
{
    UINoticeController notice;
    UIReturnToFirstController returnToFirst;
    VisualElement MainBG;
    [SerializeField]
    GameObject[] Characters;
    [SerializeField]
    AnimatorController[] AnimatorControllers;

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
            StepInitHandler[_bgState].Invoke();

            returnToFirst.MoveImgs(value, () =>
            {
                MainBG.ClearClassList();
                MainBG.AddToClassList("MainBG-Base");
                MainBG.AddToClassList($"MainBG-{value.ToString()}");
                CurAudioClip = ScrollUpAudio;
            });
        }
    }
    Dictionary<BGState, Action> StepInitHandler = new Dictionary<BGState, Action>();
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
            StepInitHandler.Add(BGState.Login, () =>
            {
            });
            #endregion
            #region WorldSelect
            AssignElements(worldSelect);
            worldSelect.channelSelect.ButtonEnterSelectedChannel.RegisterCallback<ClickEvent>(EnterChannel);
            InitializeServer();
            StepInitHandler.Add(BGState.WorldSelect, () =>
            {
                worldSelect.channelSelect.ImgChannelSelectMain.AddToClassList("ChannelSelect-Hide");
                worldSelect.channelSelect.ImgChannelScroll.style.backgroundImage = AnimationSet[0].SpriteInfo[0].Img;
                StepInitHandler[BGState.CharacterSelect].Invoke();
            });
            #endregion
            #region CharacterSelect
            AssignElements(characterSelect.navi);
            characterSelect.characterList.ContainerCharacterList = _root.Q("Container-CharacterList");
            for (int i = 1; i <= characterCount; i++)
            {
                Button button = _root.Q<Button>($"Button-Character_{i}");

                button.UnregisterCallback<MouseOverEvent>(OnMouseOverPlay);
                button.UnregisterCallback<ClickEvent>(OnMouseClickPlay);
                characterButtons.Add(button);
                characterSelect.characterList.characterPanel[i - 1] = new CharacterStatusPanel();
                AssignElements(characterSelect.characterList.characterPanel[i - 1], button);

                button.RegisterCallback<ClickEvent>(ClickCharacter);
            }

            characterSelect.navi.ButtonSelectCharacter.RegisterCallback<ClickEvent>(HandleSelectChar);
            characterSelect.navi.ButtonCreateCharacter.RegisterCallback<ClickEvent>(HandleCreateChar);
            characterSelect.navi.ButtonDeleteCharacter.RegisterCallback<ClickEvent>(HandleDeleteChar);
            StepInitHandler.Add(BGState.CharacterSelect, () =>
            {
                for (int i = 1; i <= characterCount; i++)
                {
                    Characters[i].SetActive(false);
                    var panel = characterSelect.characterList.characterPanel[i - 1];
                    panel.ImgCharacterSelectEffect.style.backgroundImage = null;
                    panel.ImgCharacterStatusScroll.style.display = new StyleEnum<DisplayStyle>(DisplayStyle.None);
                    panel.ImgCharacterStatusMain.AddToClassList("StatusMain-Hide");
                }
                StepInitHandler[BGState.CreateCharacter].Invoke();
            });
            #endregion
            #region CreateCharacter
            AssignElements(createCharacter);

            createCharacter.ButtonCharacterCreate.RegisterCallback<ClickEvent>(FormCharacter);
            createCharacter.characterAbility.ButtonNameCheck.RegisterCallback<ClickEvent>(CheckName);
            createCharacter.characterAbility.ButtonDice.RegisterCallback<ClickEvent>(GenerateAbilities);
            createCharacter.characterAbility.ButtonDice.UnregisterCallback<MouseOverEvent>(OnMouseOverPlay);

            GenerateAbilities();
            InitializeCharList();
            StepInitHandler.Add(BGState.CreateCharacter, () =>
            {
                Characters[0].SetActive(false); // 임시
            });
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
        else if (pkt is SC_CharacterList characterList)
        {
            switch (characterList.Ok)
            {
                case CharacterListError.SUCCESS:
                    BackgroundState = BGState.CharacterSelect;
                    ShowCharacters(characterList);
                    break;
                case CharacterListError.UNKNOWN:
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        else if (pkt is SC_CharacterSelect characterSelect)
        {
            string pos = characterSelect.CharInfo.Value.LastPos;
            var json = JObject.Parse(pos);
            byte mapId = json["map"].Value<byte>();

            FlatBufferBuilder builder = new FlatBufferBuilder(50);
            var data = C_EnterMap.CreateC_EnterMap(builder, mapId);
            var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_EnterMap);
            Manager.Network.Send(packet);
        }
        else if (pkt is SC_CharacterDelete characterDelete)
        {
            switch (characterDelete.Ok)
            {
                case CharacterDeleteError.SUCCESS:
                    FlatBufferBuilder builder = new FlatBufferBuilder(1);
                    C_CharacterList.StartC_CharacterList(builder);
                    var data = C_CharacterList.EndC_CharacterList(builder);
                    var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_CharacterList);
                    Manager.Network.Send(packet);
                    break;
                case CharacterDeleteError.UNKNOWN:
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        #endregion
        #region CreateCharacter
        else if (pkt is SC_CheckName checkName)
        {
            switch (checkName.Ok)
            {
                case CheckNameError.SUCCESS:
                    approvedName = createCharacter.characterAbility.TextFieldCharacterName.text;
                    NoticeState = PopupState.CanUseName;
                    break;
                case CheckNameError.OVERLAPPED:
                    approvedName = null;
                    NoticeState = PopupState.BeingUsed;
                    break;
                case CheckNameError.UNKNOWN:
                    approvedName = null;
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        else if (pkt is SC_CreateCharacter createCharacter)
        {
            switch (createCharacter.Ok)
            {
                case CreateCharacterError.SUCCESS:
                    FlatBufferBuilder builder = new FlatBufferBuilder(1);
                    C_CharacterList.StartC_CharacterList(builder);
                    var data = C_CharacterList.EndC_CharacterList(builder);
                    var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_CharacterList);
                    Manager.Network.Send(packet);
                    break;
                case CreateCharacterError.OVERLAPPED:
                    approvedName = null;
                    NoticeState = PopupState.BeingUsed;
                    break;
                case CreateCharacterError.UNKNOWN:
                    approvedName = null;
                    NoticeState = PopupState.Unknown;
                    break;
            }
        }
        #endregion
    }
}
