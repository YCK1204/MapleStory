using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class UIBaseController : BaseMonobehaviour
{
    [System.Serializable]
    public class SpriteCycle
    {
        public Texture2D[] Imgs;
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
        InitHandler<Button>("Button-", _buttons, (b) =>
        {
            b.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
            b.RegisterCallback<ClickEvent>(OnMouseClickPlay);
        });
        InitHandler<TextField>("TextField-", _textFields);
        InitHandler<ListView>("ListView-", _listViews);
        InitHandler<ScrollView>("ScrollView-", _scrollViews);
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
        foreach (var img in data.Imgs)
        {
            target.style.backgroundImage = img;
            yield return new WaitForSeconds(data.Tick);
        }
        callback?.Invoke();
        StopCoroutine("ImgAnimation");
    }
    protected void StartImgAnimation(VisualElement target, int dataIdx, Action callback = null)
    {
        StartCoroutine(ImgAnimation(target, dataIdx, callback));
    }
}
