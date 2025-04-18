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
}
