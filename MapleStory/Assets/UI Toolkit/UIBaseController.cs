using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class UIBaseController : BaseMonobehaviour
{
    [SerializeField]
    string DefaultMouseOverAudioPath = "Audio/BtMouseOver";
    [SerializeField]
    string DefaultMouseClickAudioPath = "Audio/BtMouseClick";


    protected UIDocument document;
    protected VisualElement _root;

    protected Dictionary<string, VisualElement> _containers = new Dictionary<string, VisualElement>();
    protected Dictionary<string, VisualElement> _imgs = new Dictionary<string, VisualElement>();
    protected Dictionary<string, Button> _buttons = new Dictionary<string, Button>();
    protected Dictionary<string, TextField> _textFields = new Dictionary<string, TextField>();
    protected Dictionary<string, ListView> _listViews = new Dictionary<string, ListView>();
    protected Dictionary<string, ScrollView> _scrollViews = new Dictionary<string, ScrollView>();

    protected AudioSource _audioSource;
    protected AudioClip DefaultOnMouseOverAudio;
    protected AudioClip DefaultOnMouseClickAudio;

    protected virtual AudioClip CurAudioClip
    {
        set
        {
            _audioSource.clip = value;
            _audioSource.Play();
        }
    }

    void Start()
    {
        Init();
    }
    void InitHandler<T1, T2>(string target, T2 destination, Action<T1> callback = null)
        where T1 : VisualElement
        where T2 : Dictionary<string, T1>
    {
        var allElements = _root.Query<T1>();

        var list = allElements.Where((e) => { return e.name.StartsWith(target); });
        list.ForEach((e) =>
        {
            destination.Add(e.name.Substring(target.Length), e);
            if (callback != null)
                callback.Invoke(e);
        });
    }
    protected virtual void Init()
    {
        DefaultOnMouseClickAudio = Manager.Resource.Load<AudioClip>(DefaultMouseClickAudioPath);
        DefaultOnMouseOverAudio = Manager.Resource.Load<AudioClip>(DefaultMouseOverAudioPath);

        _audioSource = gameObject.AddComponent<AudioSource>();
        document = GetComponent<UIDocument>();

        _root = document.rootVisualElement;
        // UI DOCUMENT 객체들 전부 가져오기
        InitHandler<VisualElement, Dictionary<string, VisualElement>>("Container-", _containers);
        InitHandler<VisualElement, Dictionary<string, VisualElement>>("Img-", _imgs);
        InitHandler<Button, Dictionary<string, Button>>("Button-", _buttons, (b) =>
        {
            b.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
            b.RegisterCallback<ClickEvent>(OnMouseClickPlay);
        });
        InitHandler<TextField, Dictionary<string, TextField>>("TextField-", _textFields);
        InitHandler<ListView, Dictionary<string, ListView>>("ListView-", _listViews);
        InitHandler<ScrollView, Dictionary<string, ScrollView>>("ScrollView-", _scrollViews);
    }
    protected virtual void OnMouseOverPlay(MouseOverEvent e) { CurAudioClip = DefaultOnMouseOverAudio; }
    protected virtual void OnMouseClickPlay(ClickEvent e) { CurAudioClip = DefaultOnMouseClickAudio; }
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
    protected int ExtractIdFromElementName(string name, char seperator = '_')
    {
        return int.Parse(name.Substring(name.IndexOf(seperator) + 1));
    }
    protected int ExtractIdFromElementName(VisualElement element, char seperator = '_')
    {
        return ExtractIdFromElementName(element.name, seperator);
    }
    protected int ExtractIdFromElementName(ClickEvent e, char seperator = '_')
    {
        return ExtractIdFromElementName(e.currentTarget as VisualElement, seperator);
    }
}
