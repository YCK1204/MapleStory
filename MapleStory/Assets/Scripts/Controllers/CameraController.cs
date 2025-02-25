using Unity.Cinemachine;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public CinemachineCamera cinemachineCamera;
    [HideInInspector]
    public GameObject Target { set { cinemachineCamera.Target.TrackingTarget = value.transform; } }
    public Collider2D Border { set { cinemachineCamera.GetComponent<CinemachineConfiner2D>().BoundingShape2D = value; } }
    public void Init()
    {
        Camera.main.cullingMask = ~0;
        DontDestroyOnLoad(gameObject);
        DontDestroyOnLoad(cinemachineCamera);
    }
}