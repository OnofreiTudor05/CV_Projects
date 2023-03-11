using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MissileBehaviourManager : MonoBehaviour
{
    public GenericMissile genericMissile;
    public GameObject GameScene;

    public void Start()
    {
        genericMissile.InitializeObject(gameObject);
    }

    public void OnCollisionEnter(Collision collision)
    {
        Debug.Log("Object " + gameObject.name + " collided with object " + collision.transform.name);

        InteractWithNearbyObjects(collision);

        NextTurn(true);
    }

    private void InteractWithNearbyObjects(Collision collision)
    {
        Collider[] colliders = Physics.OverlapSphere(gameObject.transform.position, genericMissile.radius);

        foreach (Collider obj in colliders)
        {
            // Deal damage
            GenericCollision genericCollision = obj.GetComponent<GenericCollision>();
            if (genericCollision != null)
            {
                genericCollision.CollidedWithMissile(collision, gameObject);
            } else
            {
                genericCollision = obj.GetComponentInChildren<GenericCollision>();
                if (genericCollision != null)
                {
                    genericCollision.CollidedWithMissile(collision, gameObject);
                }
            }

            // Push object
            // WORK IN PROGRESS
           /* Rigidbody rigidbody = obj.GetComponent<Rigidbody>();
            if(rigidbody != null)
            {
                rigidbody.AddExplosionForce(genericMissile.damage * 100f, transform.position, genericMissile.radius);
            }*/
        }
    }

    public void Update()
    {
        if(transform.position.y < -10)
        {
            NextTurn();
        }
    }

    private void NextTurn(bool withEffect = false)
    {
        Explode(withEffect);
    }

    private void Explode(bool withEffect = false)
    {
        if (withEffect)
        {
            Instantiate(genericMissile.explosionEffect, transform.position, transform.rotation);
        }

        genericMissile.ResetMissilePosition(gameObject);

        GameScene.GetComponentInChildren<GameManager>().EndTurn();
    }
}
