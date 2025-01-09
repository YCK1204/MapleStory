using Newtonsoft.Json;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;
using Random = UnityEngine.Random;

public partial class UIPrevInGameController : UIBaseController
{
    [SerializeField]
    GameObject[] Characters;
    class CharacterAbility
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
        public VisualElement ImgCharacterInfo;
        public Button ButtonCharacterCreate;
        public ScrollView ScrollViewCharacterList;
        public CharacterAbility characterAbility = new CharacterAbility();
    }
    #region CharacterJson
    [SerializeField]
    TextAsset CharacterJson;
    struct JCharacterInfo
    {
        public int id { get; set; }
        public string name { get; set; }
    }
    struct JCharacterInfos
    {
        public JCharacterInfo[] characters;
    }
    #endregion
    CreateCharacter createCharacter = new CreateCharacter();
    List<Button> _characterButtons = new List<Button>();
    private void InitializeCharList()
    {
        JCharacterInfos characterInfos = JsonConvert.DeserializeObject<JCharacterInfos>(CharacterJson.ToString());

        for (int i = 0; i < characterInfos.characters.Length; i++)
        {
            JCharacterInfo character = characterInfos.characters[i];

            Button button = CreateButton(character.name);
            button.name = character.name;
            button.AddToClassList("CharacterList-Base");
            button.AddToClassList($"Button-{character.name}");

            button.RegisterCallback<ClickEvent>((e) => {
                createCharacter.ImgCharacterInfo.ClearClassList();
                createCharacter.ImgCharacterInfo.AddToClassList($"CharacterInfo-{character.name}");
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
            //CharacterList.Add(button);
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
        // 서버에게 네임 중복 확인 요청
    }
    private void FormCharacter(ClickEvent e)
    {
        // 서버에게 캐릭터 생성 요청
    }
}
