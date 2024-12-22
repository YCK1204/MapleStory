using Newtonsoft.Json;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class UINewCharacterController : UIBaseController
{
    ScrollView _charListScrollView;
    VisualElement _background;
    CharInfo _charInfo = new CharInfo();

    #region CharacterJson
    TextAsset CharacterJson;
    Dictionary<int, Character> _characters = new Dictionary<int, Character>();
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
    struct CharInfo // 캐릭터 선택했을 때 나오는 이미지 // 기본값은 scrollview의 0번째 인덱스의 캐릭터 정보
    {
        public VisualElement BG;
        public VisualElement InfoStr;
        public Button SelectChar;
        public VisualElement CharType;
    }
    protected override void Init()
    {
        base.Init();
        _charListScrollView = _scrollViews["CharList"];
        _background = _imgs["CharBG"];

        _charInfo.BG = _imgs["CharInfoBG"];
        _charInfo.InfoStr = _imgs["CharInfoStr"];
        _charInfo.SelectChar = _buttons["SelectChar"];
        _charInfo.CharType = _imgs["CharType"];
        InitializeCharImg();
    }
    private void InitializeCharImg()
    {
        string jsonStr = CharacterJson.ToString();
        var json = JsonConvert.DeserializeObject<Characters>(jsonStr);

        foreach (var character in json.characters)
        {
            _characters[character.id] = character;

            Button charBtn = CreateButton();
            charBtn.AddToClassList("Button-CharacterBase");
            charBtn.AddToClassList($"Button-{character.name}");
            charBtn.name = $"Button-Char_{character.id}";
            charBtn.RegisterCallback<ClickEvent>(ChangeImgs);
        }
    }
    private void ChangeImgs(ClickEvent e)
    {
        int id = ExtractIdFromElementName(e);
        var character = _characters[id];

        _background.ClearClassList();
        _background.AddToClassList($"CharBG-{character.name}");

        _charInfo.InfoStr.ClearClassList();
        _charInfo.InfoStr.AddToClassList($"CharInfo-{character.name}");

        _charInfo.CharType.ClearClassList();
        _charInfo.CharType.AddToClassList($"CharType-{character.name}");
    }
}