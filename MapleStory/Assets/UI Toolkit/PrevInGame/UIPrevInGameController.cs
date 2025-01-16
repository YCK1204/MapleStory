using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;

public partial class UIPrevInGameController : UIBaseController
{
    [SerializeField]
    RawImage[] RawImages;
    [SerializeField]
    Canvas canvas;
    
    private void LateUpdate()
    {
        for (int i = 0; i <  characterStatuses.Count; i++)
        {
            var characterPanel = characterSelect.characterList.characterPanel[i];
            Rect worldBound = characterPanel.ImgCharacter.worldBound;

            Vector2 screenPosition = worldBound.position;
            // Canvas 높이 가져오기 (Y 좌표 반전용)
            float canvasHeight = canvas.GetComponent<RectTransform>().rect.height;

            // Y 좌표 반전 (UI Toolkit -> UGUI)
            screenPosition.y = canvasHeight - screenPosition.y;

            // 화면 좌표를 RectTransform 좌표로 변환
            RectTransformUtility.ScreenPointToLocalPointInRectangle(
                canvas.transform as RectTransform,
                screenPosition,
                canvas.worldCamera,
                out Vector2 localPosition);
            localPosition.x += 50f;
            localPosition.y -= 130f;
            // RawImage의 위치 업데이트
            RawImages[i].rectTransform.localPosition = localPosition;
        }
    }
}
