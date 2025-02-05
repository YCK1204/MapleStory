using Unity.VisualScripting;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    [SerializeField]
    float Speed = 3f;
    [SerializeField]
    float JumpForce = 3f;
    Rigidbody2D rb;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.LeftArrow))
            Move(Vector3.left);
        if (Input.GetKey(KeyCode.RightArrow))
            Move(Vector3.right);
        if (Input.GetKey(KeyCode.DownArrow))
        {

        }
        if (Input.GetKey(KeyCode.UpArrow))
            Jump();
    }
    private void OnCollisionEnter2D(Collision2D collision)
    {
        isJump = false;
        rb.angularVelocity = 0;
    }
    void Move(Vector3 dir)
    {
        transform.position += Speed * Time.deltaTime * dir;
    }
    bool isJump = false;
    void Jump()
    {
        if (isJump == true || rb.linearVelocity.magnitude > .01f)
            return;
        rb.AddForce(Vector3.up * JumpForce, (ForceMode2D)ForceMode.Impulse);
        isJump = true;
    }
}
