using System;
using System.Collections;
using UnityEngine;

public class BaseMonobehaviour : MonoBehaviour
{
    IEnumerator InvokeAction(Action action, float time)
    {
        yield return new WaitForSeconds(time);
        action.Invoke();
        StopCoroutine("InvokeAction");
    }
    public void Invoke(Action action, float time = 0f)
    {
        StartCoroutine(InvokeAction(action, time));
    }
    private IEnumerator FadeInInvoke(SpriteRenderer renderer, float duration)
    {
        Color color = renderer.color;
        float elapsed = 0f;

        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            float alpha = Mathf.Clamp01(elapsed / duration);
            color.a = alpha;
            renderer.color = color;
            yield return null;
        }

        color.a = 1f;
        renderer.color = color;
        StopCoroutine(FadeInInvoke(renderer, duration));
    }
    public void FadeIn(float duration)
    {
        var renderer = GetComponent<SpriteRenderer>();
        StartCoroutine(FadeInInvoke(renderer, duration));
    }
}
