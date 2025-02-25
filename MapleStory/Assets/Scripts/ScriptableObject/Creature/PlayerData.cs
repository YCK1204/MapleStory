using UnityEngine;

[CreateAssetMenu(fileName = "Player Data", menuName = "Scriptable Object/Player Data", order = 1)]
public class PlayerData : ScriptableObject
{
    [SerializeField]
    public RuntimeAnimatorController AnimatorController;
}
