using System.Collections;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;

public class IntroScene : BaseScene
{
    VideoPlayer videoPlayer;
    [SerializeField]
    Image img;
    [SerializeField]
    RawImage rawImage;
    void Start()
    {
        videoPlayer = GetComponent<VideoPlayer>();
        videoPlayer.Prepare();
        videoPlayer.prepareCompleted += (e) => 
        { 
            Invoke(() => e.Play(), 1f); 
        };
        videoPlayer.loopPointReached += (e) =>
        {
            rawImage.gameObject.SetActive(false);
            StartCoroutine(CoStartIntro());
        };
    }
    IEnumerator CoStartIntro()
    {
        float a = 0;
        float tick = .05f;
        while (true)
        {
            img.color = new Color(1, 1, 1, a);
            a += .05f;
            yield return new WaitForSeconds(tick);
            if (a >= 1)
                break;
        }

        a = 1f;

        yield return new WaitForSeconds(2f);

        while (true)
        {
            if (a <= 0)
                break;
            img.color = new Color(1, 1, 1, a);
            a -= .05f;
            yield return new WaitForSeconds(tick);
        }
        Manager.Scene.LoadScene("Login");
        StopCoroutine(CoStartIntro());
    }

    public override void OnRecvPacket<T>(T pkt)
    {
    }
}