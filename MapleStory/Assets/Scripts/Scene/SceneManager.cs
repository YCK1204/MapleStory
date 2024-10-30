using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneManagerEx : IManager
{
    public BaseScene CurScene { get; set; }
    public void Clear()
    {
    }
    public void Init()
    {
    }
    public void LoadScene(int sceneId)
    {
        SceneManager.LoadScene(sceneId);
    }
    public void LoadScene(string sceneName)
    {
        SceneManager.LoadScene(sceneName);
    }
}