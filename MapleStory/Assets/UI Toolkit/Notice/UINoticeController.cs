using Google.FlatBuffers;
using System.Xml;
using UnityEngine;
using UnityEngine.UIElements;
using static UINoticeController;

public class UINoticeController : UIBaseController
{
    class Notice
    {
        public VisualElement ImgText;
        public Button ButtonOk;
        public Button ButtonOkCenter;
        public Button ButtonCancel;
    }
    Notice notice = new Notice();
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
        Unknown,                    // �� �� ���� ����
        NoMoreCreateCharacter,      // ĳ���� �� �̻� ������
        CheckName,                  // ���� check�ض�
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
            notice.ButtonOk.style.display = DisplayStyle.None;
            notice.ButtonOkCenter.style.display = DisplayStyle.None;
            notice.ButtonCancel.style.display = DisplayStyle.None;
            if (value <= PopupState.CharacterDelete)
            {
                notice.ButtonOk.style.display = DisplayStyle.Flex;
                notice.ButtonCancel.style.display = DisplayStyle.Flex;
            }
            else
            {
                notice.ButtonOkCenter.style.display = DisplayStyle.Flex;
            }
            notice.ImgText.ClearClassList();
            notice.ImgText.AddToClassList(className);
            notice.ImgText.style.display = DisplayStyle.Flex;
        }
    }

    [SerializeField]
    UIPrevInGameController _pg;
    protected override void Init()
    {
        base.Init();

        AssignElements(notice);
        notice.ButtonOk.RegisterCallback<ClickEvent>(HandleOkBtn);
        notice.ButtonCancel.RegisterCallback<ClickEvent>((e) => { notice.ImgText.style.display = DisplayStyle.None; });
        notice.ButtonOkCenter.RegisterCallback<ClickEvent>((e) => { notice.ImgText.style.display = DisplayStyle.None; });
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
                var charData = _pg.characterStatuses[_pg.CurId - 1];
                FlatBufferBuilder builder = new FlatBufferBuilder(20);
                var data = C_CharacterDelete.CreateC_CharacterDelete(builder, charData.charId);
                var pkt = Manager.Packet.CreatePacket(data, builder, PacketType.C_CharacterDelete);
                Manager.Network.Send(pkt);
                break;
        }
        notice.ImgText.style.display = DisplayStyle.None;
    }
}


