using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;
using UnityEngine.UIElements;
using static UnityEngine.GraphicsBuffer;

public class UIBaseController : BaseMonobehaviour
{
    string[] conventionNames = { "Container", "Img", "Button", "Label", "GroupBox", "TextField", "ScrollView" };

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
    [SerializeField]
    protected AudioClip DefaultOnMouseOverAudio;
    [SerializeField]
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
    protected virtual void Init()
    {
        _audioSource = gameObject.AddComponent<AudioSource>();
        if (DefaultOnMouseOverAudio == null)
            DefaultOnMouseOverAudio = Manager.Resource.Load<AudioClip>("Common/Sounds/BtMouseOver");
        if (DefaultOnMouseClickAudio == null)
            DefaultOnMouseClickAudio = Manager.Resource.Load<AudioClip>("Common/Sounds/BtMouseClick");
        document = GetComponent<UIDocument>();
        _root = document.rootVisualElement;

        var allElements = _root.Query<Button>();
        var list = allElements.Where((e) => { return e.name.StartsWith("Button-"); });
        list.ForEach((e) =>
        {
            e.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
            e.RegisterCallback<ClickEvent>(OnMouseClickPlay);
        });
    }
    protected virtual void OnMouseOverPlay(MouseOverEvent e) { CurAudioClip = DefaultOnMouseOverAudio; }
    protected virtual void OnMouseClickPlay(ClickEvent e) { CurAudioClip = DefaultOnMouseClickAudio; }
    protected Button CreateButton(string name = "")
    {
        var button = new Button() { name = name };

        button.AddToClassList("Button-Base");
        button.RegisterCallback<ClickEvent>(OnMouseClickPlay);
        button.RegisterCallback<MouseOverEvent>(OnMouseOverPlay);
        return button;
    }
    #region Element ID ÃßÃâ
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
    #endregion
    #region Animation
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
    #endregion
    string AssignElementsHandler(FieldInfo field)
    {
        foreach (var conventionName in conventionNames)
        {
            if (field.Name.Contains(conventionName))
            {
                string name = field.Name.Substring(conventionName.Length);
                return conventionName + "-" + name;
            }
        }
        return null;
    }
    protected void AssignElements(object container, VisualElement parent = null)
    {
        Type type = container.GetType();

        foreach (var field in type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance))
        {
            string elementName = AssignElementsHandler(field);

            if (elementName == null)
                AssignElements(field.GetValue(container));
            else
            {
                VisualElement element = null;
                if (parent == null)
                    element = _root.Q(elementName);
                else
                    element = parent.Q(elementName);
                field.SetValue(container, element);
            }
        }
    }
}
