using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class UIBaseController : BaseMonobehaviour
{
    [System.Serializable]
    public class SpriteInfo
    {
        public Texture2D Img;
        public Vector2 Size = Vector2.zero;
    }
    [System.Serializable]
    public class SpriteCycle
    {
        public SpriteInfo[] SpriteInfo;
        public float Tick;
    }
    [SerializeField]
    public SpriteCycle[] AnimationSet;
    [SerializeField]
    AudioClip DefaultMouseOverAudio;
    [SerializeField]
    AudioClip DefaultMouseClickAudio;


    protected UIDocument document;
    protected VisualElement _root;

    protected Dictionary<string, VisualElement> _containers = new Dictionary<string, VisualElement>();
    protected Dictionary<string, VisualElement> _imgs = new Dictionary<string, VisualElement>();
    protected Dictionary<string, Button> _buttons = new Dictionary<string, Button>();
    protected Dictionary<string, TextField> _textFields = new Dictionary<string, TextField>();
    protected Dictionary<string, Label> _labels = new Dictionary<string, Label>();
    protected Dictionary<string, ListView> _listViews = new Dictionary<string, ListView>();
    protected Dictionary<string, ScrollView> _scrollViews = new Dictionary<string, ScrollView>();
    protected Dictionary<string, GroupBox> _groupBoxes = new Dictionary<string, GroupBox>();

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
    void InitHandler<T1>(string target, Dictionary<string, T1> destination, Action<T1> callback = null) where T1 : VisualElement
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
        _audioSource = gameObject.AddComponent<AudioSource>();
        if (DefaultOnMouseOverAudio == null)
            DefaultOnMouseOverAudio = Manager.Resource.Load<AudioClip>("Common/Sounds/BtMouseOver");
        if (DefaultOnMouseClickAudio == null)
            DefaultOnMouseClickAudio = Manager.Resource.Load<AudioClip>("Common/Sounds/BtMouseClick");
        document = GetComponent<UIDocument>();

        _root = document.rootVisualElement;
        // UI DOCUMENT 객체들 전부 가져오기
        InitHandler<VisualElement>("Container-", _containers);
        InitHandler<VisualElement>("Img-", _imgs);
        InitHandler<Label>("Label-", _labels);
        InitHandler<Button>("Button-", _buttons, (b) =>
        {
            b.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
            b.RegisterCallback<ClickEvent>(OnMouseClickPlay);
        });
        InitHandler<TextField>("TextField-", _textFields);
        InitHandler<ListView>("ListView-", _listViews);
        InitHandler<ScrollView>("ScrollView-", _scrollViews);
        InitHandler<GroupBox>("GroupBox-", _groupBoxes);
    }
    protected virtual void OnMouseOverPlay(MouseOverEvent e) { CurAudioClip = DefaultOnMouseOverAudio; }
    protected virtual void OnMouseClickPlay(ClickEvent e) { CurAudioClip = DefaultOnMouseClickAudio; }
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
    protected Button CreateButton(string name = "")
    {
        var button = new Button() { name = name };

        button.AddToClassList("Button-Base");
        button.RegisterCallback<ClickEvent>(OnMouseClickPlay);
        button.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
        return button;
    }
    IEnumerator ImgAnimation(VisualElement target, int dataIdx, Action callback)
    {
        SpriteCycle data = AnimationSet[dataIdx];
        StyleLength originalX = target.style.width;
        StyleLength originalY = target.style.height;

        foreach (var info in data.SpriteInfo)
        {
            target.style.backgroundImage = info.Img;
            target.style.width = (info.Size.x != 0) ? new Length(info.Size.x, LengthUnit.Percent) : originalX;
            target.style.height = (info.Size.y != 0) ? new Length(info.Size.y, LengthUnit.Percent) : originalY;
            yield return new WaitForSeconds(data.Tick);
        }
        callback?.Invoke();
        StopCoroutine(ImgAnimation(target, dataIdx, callback));
    }
    protected void StartImgAnimation(VisualElement target, int dataIdx, Action callback = null)
    {
        StartCoroutine(ImgAnimation(target, dataIdx, callback));
    }
    IEnumerator RepeatCallback(Action callback, float t, float interval)
    {
        float time = Time.time;
        while (true)
        {
            if (Time.time - time > interval)
                break;
            callback?.Invoke();
            yield return new WaitForSeconds(t);
        }
        StopCoroutine(RepeatCallback(callback, t, interval));
    }
    protected void StartRepeat(Action callback, float t, float interval)
    {
        StartCoroutine(RepeatCallback(callback, t, interval));
    }
}
