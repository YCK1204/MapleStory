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
            // Canvas ���� �������� (Y ��ǥ ������)
            float canvasHeight = canvas.GetComponent<RectTransform>().rect.height;

            // Y ��ǥ ���� (UI Toolkit -> UGUI)
            screenPosition.y = canvasHeight - screenPosition.y;

            // ȭ�� ��ǥ�� RectTransform ��ǥ�� ��ȯ
            RectTransformUtility.ScreenPointToLocalPointInRectangle(
                canvas.transform as RectTransform,
                screenPosition,
                canvas.worldCamera,
                out Vector2 localPosition);
            localPosition.x += 50f;
            localPosition.y -= 130f;
            // RawImage�� ��ġ ������Ʈ
            RawImages[i].rectTransform.localPosition = localPosition;
        }
    }
}
