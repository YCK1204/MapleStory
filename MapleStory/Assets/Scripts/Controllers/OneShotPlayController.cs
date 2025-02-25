using System.Collections;
using UnityEngine;

public class OneShotPlayController : MonoBehaviour
{
    AudioSource audioSource;
    public AudioClip audioClip;
    void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }
    public void Play()
    {
        audioSource.Play();
        StartCoroutine(WaitForAudioToEnd());
    }
    IEnumerator WaitForAudioToEnd()
    {
        while (true)
        {
            yield return new WaitForSeconds(.1f);
            if (audioSource.isPlaying == false)
                break;
        }
        GameObject.Destroy(gameObject);
    }
}
