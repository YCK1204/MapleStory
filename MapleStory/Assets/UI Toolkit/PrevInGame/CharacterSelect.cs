using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public partial class UIPrevInGameController : UIBaseController
{
    static readonly int characterCount = 3;
    [SerializeField]
    GameObject CharacterSelect1;
    [SerializeField]
    GameObject CharacterSelect2;
    [SerializeField]
    GameObject CharacterSelect3;
    List<Button> characterButtons = new List<Button>();
    class CharacterStatusPanel
    {
        public VisualElement ImgCharacter;
        public VisualElement ImgCharacterSelectEffect;
        public VisualElement ImgCharacterStatusScroll;
        public VisualElement ImgCharacterStatusMain;
        public Label LabelStatusJob;
        public GroupBox GroupBoxCharacterStatus1;
        public Label LabelStatusLV;
        public Label LabelStatusSTR;
        public Label LabelStatusDEX;
        public GroupBox GroupBoxCharacterStatus2;
        public Label LabelStatusFAME;
        public Label LabelStatusINT;
        public Label LabelStatusLUK;
    }
    class CharacterList
    {
        public VisualElement ContainerCharacterList;
        public CharacterStatusPanel[] characterPanel = new CharacterStatusPanel[characterCount];
    }
    class Navi
    {
        public VisualElement ImgNavi;
        public Button ButtonSelectCharacter;
        public Button ButtonCreateCharacter;
        public Button ButtonDeleteCharacter;
    }
    class CharacterSelect
    {
        public Navi navi = new Navi();
        public CharacterList characterList = new CharacterList();
    }
    CharacterSelect characterSelect = new CharacterSelect();
    VisualElement CurSelectCharacter;
    private void AssignCharacterStatusPanel(CharacterStatusPanel[] statuses)
    {
    }
    private void HandleSelectChar(ClickEvent e)
    {
        if (CurSelectCharacter == null)
            return;
        // 게임 시작
    }
    private void HandleCreateChar(ClickEvent e) { 
        BackgroundState = BGState.CreateCharacter;
        Button firstBtn = _characterButtons[0];
        firstBtn.Focus();
    }
    private void HandleDeleteChar(ClickEvent e)
    {
        if (CurSelectCharacter == null)
            return;
        notice.State = UINoticeController.PopupState.CharacterDelete;
    }
}
