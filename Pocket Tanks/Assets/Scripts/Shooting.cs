using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class Shooting : MonoBehaviour
{
    [SerializeField]
    private GameObject turret;

    [SerializeField]
    private GameObject cannon;

    [SerializeField]
    private InputAction shootingControls;

    Vector2 input = new Vector2();
    Vector3 finalInput = new Vector3();

    float speed = 15f;
    void Start()
    {
        
    }

    void Update()
    {
        input = shootingControls.ReadValue<Vector2>();
        if (input.x != 0 && input.y != 0) {
            input.y = 0;
        }
        if (cannon.transform.rotation.x < -0.33f){
            finalInput.x = 0f;
        }
        else{
            finalInput.x = -input.y;
        }
        finalInput.y = input.x;
        print(finalInput);

        if (finalInput.magnitude > 0.1f) {
            cannon.transform.Rotate(finalInput * Time.deltaTime * speed);
        }
    }

    private void OnDisable()
    {
        shootingControls.Disable();
    }

    private void OnEnable()
    {
        shootingControls.Enable();
    }
}
