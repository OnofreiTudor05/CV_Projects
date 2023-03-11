using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement : MonoBehaviour
{
    [SerializeField] 
    CharacterController movementController;

    [SerializeField]
    float speed = 5f;

    Vector2 horizontalInput;

    private void Update()
    {
        Vector3 horizontalVelocity = (transform.right * horizontalInput.normalized.x + transform.forward * horizontalInput.normalized.y) * speed;
        movementController.Move(horizontalVelocity * Time.deltaTime);
    }

    public void ReceiveInput(Vector2 _horizontal) {
        horizontalInput = _horizontal;
        print(horizontalInput);
    }
    
}
