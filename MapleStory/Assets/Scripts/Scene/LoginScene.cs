using System;
using UnityEngine;
using UnityEngine.UIElements;


public class LoginScene : BaseScene
{
    VisualElement root;
    AudioSource audioSource;
    [SerializeField]
    AudioClip hoverClip;
    [SerializeField]
    AudioClip activeClip;
    MainInput mainInput = new MainInput();
    Popup popup = new Popup();

    enum PopupTextState
    {
        CreatedId,          // normal
        RegisteredId,       // normal
        GameExit,           // normal
        Length,             // normal
        InputPattern,       // normal
        Id,                 // crash
        Password,           // crash
        Unknown,            // crash
    }
    enum PopupBackgroundState
    {
        Normal,
        Crash,
        ConnectingServer,
        GettingCharacter,
    }
    PopupTextState _textState;
    PopupTextState popupTextState
    {
        get { return _textState; }
        set
        {
            string prevClassName = _textState.ToString();
            bool crash = false;

            _textState = value;
            string curClassName = _textState.ToString();
            switch (value)
            {
                case PopupTextState.Id:
                case PopupTextState.Password:
                case PopupTextState.Unknown:
                    crash = true;
                    break;
            }
            if (crash == true)
                popupBackgroundState = PopupBackgroundState.Crash;
            else
                popupBackgroundState = PopupBackgroundState.Normal;
            string proText = "PopupText";
            if (prevClassName != curClassName)
                popup.text.RemoveFromClassList($"{proText}-{prevClassName}");
            popup.text.AddToClassList($"{proText}-{curClassName}");
        }
    }
    PopupBackgroundState _backgroundState;
    PopupBackgroundState popupBackgroundState
    {
        get { return _backgroundState; }
        set
        {
            string prevClassName = _backgroundState.ToString();
            _backgroundState = value;
            string curClassName = _backgroundState.ToString();
            string proText = "PopupBG";
            popup.background.RemoveFromClassList($"{proText}-{prevClassName}");
            popup.background.AddToClassList($"{proText}-{curClassName}");
            popup.container.RemoveFromClassList("Popup-Hide");
            Invoke(() => { popup.container.visible = true; }, 0f);
        }
    }

    class MainInput
    {
        public Button login;
        public Button gameExit;
        public Button signUp;
        public TextField id;
        public TextField password;
        public void Init(VisualElement root)
        {
            login = root.Q<Button>("Button-Login");
            gameExit = root.Q<Button>("Button-GameExit");
            signUp = root.Q<Button>("Button-SignUp");
            id = root.Q<TextField>("TextField-Id");
            password = root.Q<TextField>("TextField-Password");
        }
    }
    class Popup
    {
        public VisualElement container;
        public VisualElement background;
        public VisualElement loadingImg;
        public VisualElement text;
        public Button ok;
        public void Init(VisualElement root)
        {
            container = root.Q<VisualElement>("Container-Popup");
            background = root.Q<VisualElement>("PopupBackground");
            loadingImg = root.Q<VisualElement>("Img-Loading");
            text = root.Q<VisualElement>("PopupText");
            ok = root.Q<Button>("Button-Ok");
        }
    }
    protected override void Init()
    {
        base.Init();
        audioSource = GetComponent<AudioSource>();

        root = document.rootVisualElement;

        mainInput.Init(root);
        popup.Init(root);

        mainInput.login.RegisterCallback<ClickEvent>(HandleLogin);
        mainInput.gameExit.RegisterCallback<ClickEvent>((e) =>
        {
            PlaySound(activeClip);
            popupTextState = PopupTextState.GameExit;
        });
        mainInput.signUp.RegisterCallback<ClickEvent>((e) => { PlaySound(activeClip); });
        popup.ok.RegisterCallback<MouseOverEvent>((e) => { PlaySound(hoverClip); });
        popup.ok.RegisterCallback<ClickEvent>((e) =>
        {
            PlaySound(activeClip);
            popup.container.AddToClassList("Popup-Hide");
            Invoke(() => { popup.container.visible = false; }, 1f);
            if (popupTextState == PopupTextState.GameExit)
            {
#if UNITY_EDITOR
                UnityEditor.EditorApplication.isPlaying = false;
#else
    Application.Quit();
#endif
            }
        });
    }
    void HandleLogin(ClickEvent e)
    {
        PlaySound(activeClip);
        string idStr = mainInput.id.text;
        string passwordStr = mainInput.password.text;
        if (idStr.Length < 4 || idStr.Length >= 20 || passwordStr.Length < 4 || passwordStr.Length >= 20)
            popupTextState = PopupTextState.Length;
        else if (idStr.isAlNum() == false || passwordStr.isAlNum() == false)
            popupTextState = PopupTextState.InputPattern;
        else
        {
            // 네트워크 결과로 수정
        }
    }
    void PlaySound(AudioClip clip)
    {
        audioSource.PlayOneShot(clip);
    }
}
