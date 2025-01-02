using UnityEngine;
using UnityEngine.UIElements;

public partial class UIPrevInGameController : UIBaseController
{
    Button SelectCharacter;
    Button CreateCharacter;
    Button DeleteCharacter;

    VisualElement CurSelectCharacter;

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
