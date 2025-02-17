using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public partial class UIPrevInGameController : UIBaseController
{
    string[] JobNames = { "AngelicBuster", "Tanjiro" };

    static readonly int characterCount = 3;
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
    public int CurId = 0;
    public class characterStatus
    {
        public UInt64 charId;
        public int level;
        public string name;
        public int charType;
        public int STR;
        public int DEX;
        public int INT;
        public int LUK;
    }
    public List<characterStatus> characterStatuses = new List<characterStatus>();
    private void HandleSelectChar(ClickEvent e)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(100);

        UInt64 selectedId = characterStatuses[CurId - 1].charId;
        var data = C_CharacterSelect.CreateC_CharacterSelect(builder, selectedId);
        var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_CharacterSelect);
        Manager.Network.Send(pkt);
    }
    private void HandleCreateChar(ClickEvent e)
    {
        BackgroundState = BGState.CreateCharacter;
        Button firstBtn = _characterButtons[0];
        firstBtn.Focus();
    }
    private void HandleDeleteChar(ClickEvent e)
    {
        notice.State = UINoticeController.PopupState.CharacterDelete;
    }
    private void ShowCharacters(SC_CharacterList characterList)
    {
        characterStatuses.Clear();
        for (int i = 0; i < characterList.ListLength; i++)
        {
            var charInfo = characterList.List(i);
            if (charInfo.HasValue)
            {
                var charVal = charInfo.Value;
                characterStatus characterStatus = new characterStatus()
                {
                    charId = charVal.CharId,
                    charType = charVal.CharType,
                    level = charVal.Level,
                    name = charVal.Name,
                };
                if (charVal.Ability.HasValue)
                {
                    characterStatus.STR = charVal.Ability.Value.STR;
                    characterStatus.DEX = charVal.Ability.Value.DEX;
                    characterStatus.INT = charVal.Ability.Value.INT;
                    characterStatus.LUK = charVal.Ability.Value.LUK;
                }
                characterStatuses.Add(characterStatus);
            }
        }
        foreach (var character in characterSelect.characterList.characterPanel)
            character.ImgCharacter.style.display = new StyleEnum<DisplayStyle>(DisplayStyle.None);

        Invoke(() =>
        {
            for (int i = 0; i < characterStatuses.Count; i++)
            {
                GameObject go = Characters[i + 1];
                go.SetActive(true);
                Animator anim = go.GetComponent<Animator>();
                //anim.runtimeAnimatorController = AnimatorControllers[characterStatuses[i].charType];
                anim.runtimeAnimatorController = AnimatorControllers[0];
            }
            CharacterSelectClear();
            for (int i = characterStatuses.Count; i < characterCount; i++)
            {
                var panel = characterSelect.characterList.characterPanel[i];

                panel.ImgCharacter.style.display = new StyleEnum<DisplayStyle>(DisplayStyle.Flex);
                Characters[i + 1].gameObject.SetActive(false);
            }
        }, .01f);
    }
    private void ClickCharacter(ClickEvent e)
    {
        int id = ExtractIdFromElementName(e);

        if (CurId == id || id > characterStatuses.Count)
            return;

        CharacterSelectClear();
        if (CurId > 0)
        {
            Animator anim = Characters[CurId].GetComponent<Animator>();
            anim.Play("Stand01");
        }

        {
            Animator anim = Characters[id].GetComponent<Animator>();
            anim.Play("Walk01");
            CurId = id;

            var panel = characterSelect.characterList.characterPanel[id - 1];
            panel.ImgCharacterStatusScroll.style.display = new StyleEnum<DisplayStyle>(DisplayStyle.Flex);
            StartImgAnimation(panel.ImgCharacterStatusScroll, 3, () =>
            {
                var status = characterStatuses[id - 1];
                Invoke(() =>
                {
                    panel.LabelStatusSTR.text = status.STR.ToString();
                    panel.LabelStatusDEX.text = status.DEX.ToString();
                    panel.LabelStatusINT.text = status.INT.ToString();
                    panel.LabelStatusLUK.text = status.LUK.ToString();
                    panel.LabelStatusLV.text = status.level.ToString();
                    panel.ImgCharacterStatusMain.RemoveFromClassList("StatusMain-Hide");
                });
            });

            StartImgAnimation(panel.ImgCharacterSelectEffect, 4);
        }
    }
    private void CharacterSelectClear(bool all = false)
    {
        foreach (var panel in characterSelect.characterList.characterPanel)
        {
            panel.ImgCharacterStatusScroll.style.display = new StyleEnum<DisplayStyle>(DisplayStyle.None);
            panel.ImgCharacterStatusMain.AddToClassList("StatusMain-Hide");
            panel.ImgCharacterSelectEffect.style.backgroundImage = null;
        }

        for (int i = 0; i < characterCount; i++)
        {
            if (all == true)
                Characters[i + 1].gameObject.SetActive(false);
            Animator anim = Characters[i + 1].GetComponent<Animator>();
            anim.Play("Stand01");
            CurId = 0;
        }
    }
}