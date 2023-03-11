using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class TankController : MonoBehaviour
{
    [SerializeField]
    private Transform cameraTransform;


    [SerializeField]
    private GameObject turret;

    [SerializeField]
    private GameObject cannon;

    [SerializeField]
    private InputAction movementControls;
    
    private float smoothRotationTime = 0.1f;
    private CharacterController controllerCharacter;

    private float speed = 5f;
    private float turnSmoothVelocity;

    Vector2 input = new Vector2(); 
    Vector3 finalInput = new Vector3();

    private void Awake()
    {
        controllerCharacter = GetComponent<CharacterController>();
    }
    private void Start()
    {

    }

    private void OnDisable()
    {
        movementControls.Disable();
    }

    private void OnEnable()
    {
        movementControls.Enable();
    }

    private void Update()
    {
        input = movementControls.ReadValue<Vector2>();
       
        finalInput.x = input.x;
        finalInput.z = input.y;

        if (finalInput.magnitude > 0.1f) {
            float angleToRotate = Mathf.Atan2(finalInput.x, finalInput.z) * Mathf.Rad2Deg + cameraTransform.eulerAngles.y;
            float smoothAngleRotation = Mathf.SmoothDampAngle(transform.eulerAngles.y, angleToRotate, ref turnSmoothVelocity, smoothRotationTime);
            
            transform.rotation = Quaternion.Euler(0f, smoothAngleRotation, 0f);
            Vector3 movingDirection = Quaternion.Euler(finalInput.x, angleToRotate, finalInput.y) * Vector3.forward;

            controllerCharacter.Move(movingDirection.normalized * Time.deltaTime * speed);

        }
    }

    public void MoveForward(InputAction.CallbackContext value) {
        transform.Translate(0f, 0f, Time.deltaTime * 10f);
    }

    public void MoveBackward(InputAction.CallbackContext value)
    {
        transform.Translate(0f, 0f, -Time.deltaTime * 10f);
    }

    public void RotateLeft()
    {
        transform.Rotate(0f, -Time.deltaTime * 10f, 0f);
    }

    public void RotateRight()
    {
        transform.Rotate(0f, Time.deltaTime * 50f, 0f);
    }

    public void RotateCannonLeft() {
        cannon.transform.Rotate(0f, -Time.deltaTime * 50f, 0f);
    }

    public void RotateCannonRight(InputAction.CallbackContext value)
    {
        cannon.transform.Rotate(0f, Time.deltaTime * 10f, 0f);
    }
    public void HeightenTurret(InputAction.CallbackContext value)
    {
        cannon.transform.Rotate(-Time.deltaTime * 4, 0f, 0f);
    }

}
