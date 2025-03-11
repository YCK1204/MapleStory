using System;
using System.Collections.Generic;
using UnityEngine;


[Serializable]
public class AudioEntry
{
    public string name;
    public AudioClip clip;
}
public class AudioManager : MonoBehaviour
{
    [SerializeField]
    List<AudioEntry> _entries = new List<AudioEntry>();
    Dictionary<string, AudioClip> _audioClips = new Dictionary<string, AudioClip>();
    private void Start()
    {
        foreach (var entry in _entries)
            _audioClips.Add(entry.name, entry.clip);
    }
    public AudioClip FindAudioClip(string name)
    {
        AudioClip clip = null;
        _audioClips.TryGetValue(name, out clip);
        return clip;
    }
    public void OneShotPlay(AudioClip clip)
    {
        var go = Manager.Resource.Instantiate("Prefabs/PlayUtil/OneShotPlay");
        var oc = go.GetComponent<OneShotPlayController>();
        oc.Play(clip);
    }
}
