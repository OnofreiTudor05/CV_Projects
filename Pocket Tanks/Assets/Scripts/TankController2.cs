using UnityEngine;
using UnityEngine.InputSystem;

public class TankController2 : MonoBehaviour
{
    private TankControls tankControls;

    [SerializeField]
    private GameObject projectile;

    [SerializeField]
    private Transform shootingPoint;

    [SerializeField]
    private GameObject turret;

    [SerializeField]
    private GameObject cannon;

    [SerializeField]
    private float maximumMovement = 10f;

    [SerializeField]
    public float currentMovement = 0f;

    private float smoothRotationTime = 0.1f;
    private CharacterController controllerCharacter;

    private float speed = 0.5f;
    private float rotationSpeed = 25f;
    private float turnSmoothVelocity;
    private float projectileVelocity = 2f;

    Vector2 input = new Vector2();
    Vector3 finalInput = new Vector3();
    
    Vector2 input1 = new Vector2();
    Vector3 finalInput1 = new Vector3();
    private void Awake()
    {
        tankControls = new TankControls();
        controllerCharacter = this.gameObject.GetComponent<CharacterController>();
    }

    private void OnEnable()
    {
        tankControls.Enable();
    }
    private void OnDisable()
    {
        tankControls.Disable();
    }
    void Update()
    {
        projectile = GameMissileHandler.currentGameMissile;

        UpdateMovement();

        UpdateCannonRotation();

        if (tankControls.Player.Shoot.triggered) {
            Shoot();
        }

    }

    private void UpdateMovement()
    {
        input = tankControls.Player.Move.ReadValue<Vector2>();

        finalInput.x = input.x;
        finalInput.z = input.y;

        if (finalInput.magnitude > 0.1f && currentMovement < maximumMovement)
        {
            float angleToRotate = Mathf.Atan2(finalInput.x, finalInput.z) * Mathf.Rad2Deg;
            float smoothAngleRotation = Mathf.SmoothDampAngle(transform.eulerAngles.y, angleToRotate, ref turnSmoothVelocity, smoothRotationTime);

            transform.rotation = Quaternion.Euler(0f, smoothAngleRotation, 0f);
            Vector3 movingDirection = Quaternion.Euler(finalInput.x, angleToRotate, finalInput.y) * Vector3.forward;

            controllerCharacter.Move(movingDirection * Time.deltaTime * speed);

            currentMovement += finalInput.magnitude;
        }
    }

    private void UpdateCannonRotation()
    {
        input1 = tankControls.Player.RotateCannon.ReadValue<Vector2>();

        finalInput1.x = -input1.y;
        finalInput1.y = input1.x;
        finalInput1.z = 0f;

        //Debug.Log("Cannon input: " + input1);

        if (finalInput1.magnitude > 0.1f)
        {
            cannon.transform.Rotate(finalInput1 * Time.deltaTime * rotationSpeed);
        }
    }

    public void Shoot()
    {
        projectile.GetComponentInChildren<Rigidbody>().useGravity = true;
        projectile.transform.position = shootingPoint.transform.position;
        projectile.transform.rotation = cannon.transform.rotation;

        projectile.GetComponentInChildren<Rigidbody>().velocity = cannon.transform.forward * projectileVelocity;
    }

}
