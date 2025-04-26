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
    IEnumerator FadeOutInvoke(SpriteRenderer renderer, float duration)
    {
        float elapsed = 0f;
        Color color = renderer.color;

        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            float alpha = Mathf.Lerp(1f, 0f, elapsed / duration);
            renderer.color = new Color(color.r, color.g, color.b, alpha);
            yield return null;
        }

        renderer.color = new Color(color.r, color.g, color.b, 0f);
        StopCoroutine(FadeOutInvoke(renderer, duration));
    }
    public void FadeOut(float duration)
    {
        var renderer = GetComponent<SpriteRenderer>();
        StartCoroutine(FadeOutInvoke(renderer, duration));
    }
}
