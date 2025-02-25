using UnityEngine;

public class BackObjectController : BaseMonobehaviour
{
    [SerializeField]
    float Speed;
    [SerializeField]
    float SpawnPosX;
    [SerializeField]
    Collider2D Target;
    void Update()
    {
        transform.position += (Vector3)Vector2.left * Speed * Time.deltaTime;
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.tag == "Border")
        {
            if (Target != null)
            {
                Vector2 rightEdge = new Vector2(Target.bounds.max.x + (Target.bounds.size.x / 2), transform.position.y);
                transform.position = rightEdge;
            }
            else
            {
                transform.position = new Vector3(SpawnPosX, transform.position.y);
            }
        }
    }
}