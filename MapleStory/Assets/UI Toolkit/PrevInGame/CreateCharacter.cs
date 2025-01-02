using Newtonsoft.Json;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;
using Random = UnityEngine.Random;

public partial class UIPrevInGameController : UIBaseController
{
    VisualElement CharacterImg;
    VisualElement CharacterInfo;
    Button CharacterCreate;

    struct CharacterStatusMain
    {
        public VisualElement Container;
        public Button DiceBtn;
        public VisualElement DiceImg;
        public GroupBox TextAbilityGroup;
        public Label STR;
        public Label DEX;
        public Label INT;
        public Label LUK;
    }
    struct CharacterStatus
    {
        public VisualElement Container;
        public Button NameCheck;
        public TextField CharacterName;
        public CharacterStatusMain Main;
    }

    #region CharacterJson

    ScrollView CharacterList;
    [SerializeField]
    TextAsset CharacterJson;
    CharacterStatus _charStatus = new CharacterStatus();
    struct Character
    {
        public int id { get; set; }
        public string name { get; set; }
    }
    struct Characters
    {
        public Character[] characters;
    }
    #endregion
    List<Button> _characterButtons = new List<Button>();
    private void InitializeCharList()
    {
        Characters characters = JsonConvert.DeserializeObject<Characters>(CharacterJson.ToString());
        foreach (Character character in characters.characters)
        {
            Button button = CreateButton(character.name);
            button.name = character.name;
            button.AddToClassList("CharacterList-Base");
            button.AddToClassList($"Button-{character.name}");

            button.RegisterCallback<ClickEvent>((e) => { 
                CharacterInfo.ClearClassList();
                CharacterInfo.AddToClassList($"CharacterInfo-{character.name}");
                // 나중에 중앙에 서 있는 캐릭터 이미지도 바꿔야함
            });
            CharacterList.Add(button);
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
            _charStatus.Main.STR.text = first.ToString();
            _charStatus.Main.DEX.text = second.ToString();
            _charStatus.Main.INT.text = third.ToString();
            _charStatus.Main.LUK.text = fourth.ToString();
        }
        else
        {
            _charStatus.Main.LUK.text = second.ToString();
            _charStatus.Main.INT.text = first.ToString();
            _charStatus.Main.DEX.text = third.ToString();
            _charStatus.Main.STR.text = fourth.ToString();
        }
        toggle = !toggle;
    }
    float tick = 0;
    const float timeInterval = .4f;

    private void GenerateAbilities(ClickEvent e)
    {
        if (Time.time - tick > timeInterval)
        {
            StartImgAnimation(_charStatus.Main.DiceImg, 2);
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
