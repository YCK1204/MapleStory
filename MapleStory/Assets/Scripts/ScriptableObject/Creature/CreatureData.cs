using UnityEngine;

[CreateAssetMenu(fileName = "Creature Data", menuName = "Scriptable Object/Creature Data", order = 1)]
public class CreatureData : ScriptableObject
{
    [SerializeField]
    public RuntimeAnimatorController AnimatorController;
}
