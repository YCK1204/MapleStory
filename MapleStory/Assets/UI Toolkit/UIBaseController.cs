using Mono.Cecil.Cil;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.UIElements;

public class UIBaseController : BaseMonobehaviour
{
    protected UIDocument document;
    protected VisualElement _root;

    protected Dictionary<string, VisualElement> _containers = new Dictionary<string, VisualElement>();
    protected Dictionary<string, VisualElement> _imgs = new Dictionary<string, VisualElement>();
    protected Dictionary<string, Button> _buttons = new Dictionary<string, Button>();
    protected Dictionary<string, TextField> _textFields = new Dictionary<string, TextField>();

    protected AudioSource _audioSource;
    [SerializeField]
    protected AudioClip DefaultOnMouseOverAudio;
    [SerializeField]
    protected AudioClip DefaultOnMouseClickAudio;
    void Start()
    {
        Init();
    }
    protected virtual void Init()
    {
        _audioSource = GetComponent<AudioSource>();
        document = GetComponent<UIDocument>();

        _root = document.rootVisualElement;
        // UI DOCUMENT 객체들 전부 가져오기
        {
            {
                var allElements = _root.Query<VisualElement>();

                var containerList = allElements.Where((e) => { return e.name.StartsWith("Container"); });
                containerList.ForEach((e) => { _containers.Add(e.name.Substring(10), e); });

                var ImgList = allElements.Where((e) => { return e.name.StartsWith("Img"); });
                ImgList.ForEach((e) => { _imgs.Add(e.name.Substring(4), e); });
            }
            {
                var allButtons = _root.Query<Button>();

                var buttonList = allButtons.Where((b) => { return b.name.StartsWith("Button"); });
                buttonList.ForEach((b) =>
                {
                    _buttons.Add(b.name.Substring(7), b);
                    b.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
                    b.RegisterCallback<ClickEvent>(OnMouseClickPlay);
                });
            }
            {
                var allTextField = _root.Query<TextField>();

                var textFieldList = allTextField.Where((t) =>{ return t.name.StartsWith("TextField"); });
                textFieldList.ForEach((t) => { _textFields.Add(t.name.Substring(10), t);});
            }
        }
    }
    protected virtual void OnMouseOverPlay(MouseOverEvent e)
    {
        _audioSource.clip = DefaultOnMouseOverAudio;
        _audioSource.Play();
    }
    protected virtual void OnMouseClickPlay(ClickEvent e)
    {
        _audioSource.clip = DefaultOnMouseClickAudio;
        _audioSource.Play();
    }
    protected void AddClassToHierarchy(VisualElement e, string className)
    {
        e.AddToClassList(className);
        foreach (var child in e.Children())
        {
            AddClassToHierarchy(child, className);
        }
    }
    protected void RemoveClassToHierarchy(VisualElement e, string className)
    {
        e.RemoveFromClassList(className);
        foreach (var child in e.Children())
        {
            RemoveClassToHierarchy(child, className);
        }
    }
}
