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
        ReturnToFirst,              // ó������ ��ư
        CharacterDelete,            // ĳ���� ���� Ȯ��
        /*------------*/
        /* Ok, Cancel */
        /*------------*/

        /*------------*/
        /*     Ok     */
        /*------------*/
            /*-------------*/
            /*    Login    */
            /*-------------*/
        InvalidId,                  // �߸��� ���̵�� ȸ������
        InvalidPassword,            // �߸��� �н����� �α���
        LoggedIn,                   // �̹� �α��ε� ���̵� or �̹� �ִ� ���̵�
        NotRegisteredId,            // ȸ������ �Ǿ����� ���� ���̵�
            /*-------------*/
            /*     Etc     */
            /*-------------*/
        BeingUsed,                  // �г��� ��� ��
        CanUseName,                 // �г��� ��� ����
        CannotCreateCharacter,      // ĳ���� �� �̻� ���� �Ұ�
        SelectChannel,              // ä�� ���� �ش޶�
        Unknown                     // �� �� ���� ����
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
                // Login���� ������
                break;
            case PopupState.CharacterDelete:
                // ĳ���� ����
                break;
        }
        Text.style.display = DisplayStyle.None;
    }
}


