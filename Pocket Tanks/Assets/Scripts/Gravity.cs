using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Gravity : MonoBehaviour
{
    void Start()
    {
        GetComponent<ConstantForce>().force = new Vector3(0, 8, 0);
    }
}
