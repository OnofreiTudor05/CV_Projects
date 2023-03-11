using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Proiectil : MonoBehaviour
{
    public float putere = 5;
    public float unghiY;
    public float unghiRotire;

    public GameObject ghiulea;
    public Transform punctLansare;

    private void Start()
    {
        transform.rotation = Quaternion.Euler(transform.rotation.eulerAngles + new Vector3(0, unghiRotire, 90-unghiY));
    }

    private void Update()
    {

        if (Input.GetKeyDown(KeyCode.Space))
        {
            GameObject CreatedCannonball = Instantiate(ghiulea, punctLansare.position, punctLansare.rotation);
            CreatedCannonball.GetComponent<Rigidbody>().velocity = punctLansare.transform.up * putere;
        }
    }

}