using Google.FlatBuffers;
using Newtonsoft.Json;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;
using Random = UnityEngine.Random;

public partial class UIPrevInGameController : UIBaseController
{
    class CharacterAbilities
    {
        public Button ButtonNameCheck;
        public VisualElement ContainerMain;
        public Button ButtonDice;
        public VisualElement ImgDice;
        public GroupBox GroupBoxAbility;
        public Label LabelAbilitySTR;
        public Label LabelAbilityDEX;
        public Label LabelAbilityINT;
        public Label LabelAbilityLUK;
        public TextField TextFieldCharacterName;
    }
    class CreateCharacter
    {
        public VisualElement ImgCharacterPreviewInfo;
        public Button ButtonCharacterCreate;
        public ScrollView ScrollViewCharacterList;
        public CharacterAbilities characterAbility = new CharacterAbilities();
    }
    #region CharacterJson
    [SerializeField]
    TextAsset CharacterJson;
    struct JCharacterPreviewInfo
    {
        public int id { get; set; }
        public string name { get; set; }
    }
    struct JCharacterPreviewInfos
    {
        public JCharacterPreviewInfo[] characters;
    }
    #endregion
    CreateCharacter createCharacter = new CreateCharacter();
    List<Button> _characterButtons = new List<Button>();
    string approvedName = null;

    private void InitializeCharList()
    {
        JCharacterPreviewInfos CharacterPreviewInfos = JsonConvert.DeserializeObject<JCharacterPreviewInfos>(CharacterJson.ToString());

        for (int i = 0; i < CharacterPreviewInfos.characters.Length; i++)
        {
            JCharacterPreviewInfo character = CharacterPreviewInfos.characters[i];

            Button button = CreateButton(character.name);
            button.name = character.name;
            button.AddToClassList("CharacterList-Base");
            button.AddToClassList($"Button-{character.name}");

            button.RegisterCallback<ClickEvent>((e) => {
                createCharacter.ImgCharacterPreviewInfo.ClearClassList();
                createCharacter.ImgCharacterPreviewInfo.AddToClassList($"CharacterPreviewInfo-{character.name}");
                // 나중에 중앙에 서 있는 캐릭터 이미지도 바꿔야함
                if (character.name == "Tanjiro") // 일단 탄지로만
                {
                    Characters[0].SetActive(true);
                }
                else
                {
                    Characters[0].SetActive(false);
                }
            });
            createCharacter.ScrollViewCharacterList.Add(button);
            _characterButtons.Add(button);
        }
    }
    #region 주사위
    bool toggle = false;
    private void GenerateAbilities()
    {
        int remainAbility = 25;
        int min = 4;
        int max = 13;

        int first = Random.Range(min, max);
        remainAbility -= first;
        max = (remainAbility - (min + min)) < max ? (remainAbility - (min + min)) : max;

        int second = Random.Range(min, max);
        remainAbility -= second;
        max = remainAbility - min;

        int third = Random.Range(min, max);
        remainAbility -= third;
        max = remainAbility;

        int fourth = Random.Range(min, max);
        fourth += (remainAbility - fourth);
        if (toggle)
        {
            createCharacter.characterAbility.LabelAbilitySTR.text = first.ToString();
            createCharacter.characterAbility.LabelAbilityDEX.text = second.ToString();
            createCharacter.characterAbility.LabelAbilityINT.text = third.ToString();
            createCharacter.characterAbility.LabelAbilityLUK.text = fourth.ToString();
        }
        else
        {
            createCharacter.characterAbility.LabelAbilityLUK.text = second.ToString();
            createCharacter.characterAbility.LabelAbilityINT.text = first.ToString();
            createCharacter.characterAbility.LabelAbilityDEX.text = third.ToString();
            createCharacter.characterAbility.LabelAbilitySTR.text = fourth.ToString();
        }
        toggle = !toggle;
    }
    float tick = 0;
    const float timeInterval = .4f;

    private void GenerateAbilities(ClickEvent e)
    {
        if (Time.time - tick > timeInterval)
        {
            StartImgAnimation(createCharacter.characterAbility.ImgDice, 2);
            StartRepeat(() =>
            {
                GenerateAbilities();
            }, .05f, timeInterval);
            Invoke(() => GenerateAbilities(), timeInterval);
            tick = Time.time;
        }
    }
    #endregion
    private void CheckName(ClickEvent e)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1024);

        string name = createCharacter.characterAbility.TextFieldCharacterName.text;
        if (name.isAlNum() == false)
        {
            NoticeState = UINoticeController.PopupState.Unknown;
            approvedName = null;
            return;
        }
        var data = C_CheckName.CreateC_CheckName(builder, builder.CreateString(name));
        var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_CheckName);
        Manager.Network.Send(pkt);
    }
    private void FormCharacter(ClickEvent e)
    {
        string name = createCharacter.characterAbility.TextFieldCharacterName.text;

        if (approvedName == null || approvedName.Equals(name) == false)
        {
            NoticeState = UINoticeController.PopupState.CheckName;
            return;
        }

        ushort STR = ushort.Parse(createCharacter.characterAbility.LabelAbilitySTR.text);
        ushort DEX = ushort.Parse(createCharacter.characterAbility.LabelAbilityDEX.text);
        ushort INT = ushort.Parse(createCharacter.characterAbility.LabelAbilityINT.text);
        ushort LUK = ushort.Parse(createCharacter.characterAbility.LabelAbilityLUK.text);

        FlatBufferBuilder builder = new FlatBufferBuilder(1024);
        var ability = CharacterAbility.CreateCharacterAbility(builder, STR, DEX, INT, LUK);
        // 나중에 캐릭터 애니메이션 배열을 통해 char_type 가져올것
        var data = C_CreateCharacter.CreateC_CreateCharacter(builder, builder.CreateString(name), 1, ability); 
        var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_CreateCharacter);
        Manager.Network.Send(pkt);
    }
}
