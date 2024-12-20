using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;

public class UINoticeController : UIBaseController
{
    VisualElement Text;
    Button Ok;
    Button OkCenter;
    Button Cancel;
    public enum PopupState
    {
        /*------------*/
        /* Ok, Cancel */
        /*------------*/
        ReturnToFirst,              // 처음으로 버튼
        CharacterDelete,            // 캐릭터 삭제 확정
        /*------------*/
        /* Ok, Cancel */
        /*------------*/

        /*------------*/
        /*     Ok     */
        /*------------*/
            /*-------------*/
            /*    Login    */
            /*-------------*/
        InvalidId,                  // 잘못된 아이디로 회원가입
        InvalidPassword,            // 잘못된 패스워드 로그인
        LoggedIn,                   // 이미 로그인된 아이디 or 이미 있는 아이디
        NotRegisteredId,            // 회원가입 되어있지 않은 아이디
            /*-------------*/
            /*     Etc     */
            /*-------------*/
        BeingUsed,                  // 닉네임 사용 중
        CanUseName,                 // 닉네임 사용 가능
        CannotCreateCharacter,      // 캐릭터 더 이상 생성 불가
        SelectChannel,              // 채널 선택 해달라
        Unknown                     // 알 수 없는 에러
        /*------------*/
        /*     Ok     */
        /*------------*/
    }

    PopupState _state;
    public PopupState State
    {
        get { return _state; }
        set
        {
            _state = value;

            string className = $"PopupImg-{value.ToString()}";
            Ok.style.display = DisplayStyle.None;
            OkCenter.style.display = DisplayStyle.None;
            Cancel.style.display = DisplayStyle.None;
            if (value <= PopupState.CharacterDelete)
            {
                Ok.style.display = DisplayStyle.Flex;
                Cancel.style.display = DisplayStyle.Flex;
            }
            else
            {
                OkCenter.style.display = DisplayStyle.Flex;
            }
            Text.ClearClassList();
            Text.AddToClassList(className);
            Text.style.display = DisplayStyle.Flex;
        }
    }
    protected override void Init()
    {
        base.Init();

        Text = _imgs["Text"];
        Ok = _buttons["Ok"];
        Cancel = _buttons["Cancel"];
        OkCenter = _buttons["OkCenter"];

        Ok.RegisterCallback<ClickEvent>(HandleOkBtn);
        Cancel.RegisterCallback<ClickEvent>((e) => { Text.style.display = DisplayStyle.None; });
        OkCenter.RegisterCallback<ClickEvent>((e) => { Text.style.display = DisplayStyle.None; });
    }
    private void HandleOkBtn(ClickEvent e)
    {
        switch (State)
        {
            case PopupState.ReturnToFirst:
                // Login으로 돌리기
                break;
            case PopupState.CharacterDelete:
                // 캐릭터 삭제
                break;
        }
        Text.style.display = DisplayStyle.None;
    }
}


