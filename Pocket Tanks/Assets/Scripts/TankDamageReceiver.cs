using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TankDamageReceiver : MonoBehaviour, GenericCollision
{
    [Range(50f, 100f)]
    public float HP = 100f;

    public void CollidedWithMissile(Collision collision, GameObject missile)
    {
        Debug.Log("Object " + gameObject.name + " HP before collision: " + HP);
        HP -= missile.GetComponent<MissileBehaviourManager>().genericMissile.damage;
        Debug.Log("Object " + gameObject.name + " HP after collision: " + HP);
    }
}
