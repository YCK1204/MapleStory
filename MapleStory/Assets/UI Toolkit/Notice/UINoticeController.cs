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
        Unknown,                    // 알 수 없는 에러
        NoMoreCreateCharacter,      // 캐릭터 더 이상 못만듬
        CheckName,                  // 네임 check해라
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
                // Login으로 돌리기
                break;
            case PopupState.CharacterDelete:
                // 캐릭터 삭제
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


