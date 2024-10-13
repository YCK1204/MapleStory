using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class CursurController : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    [SerializeField]
    Texture2D DefaultCursur;
    [SerializeField]
    Texture2D ClickedCursur;
    [SerializeField]
    Texture2D HoverCursur;

    public void OnPointerEnter(PointerEventData eventData)
    {
    }

    public void OnPointerExit(PointerEventData eventData)
    {
    }
}
