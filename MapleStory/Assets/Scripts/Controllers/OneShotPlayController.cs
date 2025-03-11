using System.Collections;
using UnityEngine;

public class OneShotPlayController : MonoBehaviour
{
    public void Play(AudioClip clip)
    {
        AudioSource audioSource = GetComponent<AudioSource>();
        audioSource.clip = clip;
        audioSource.Play();
        StartCoroutine(WaitForAudioToEnd(clip.length));
    }
    IEnumerator WaitForAudioToEnd(float time)
    {
        yield return new WaitForSeconds(time);
        GameObject.Destroy(gameObject);
    }
}
