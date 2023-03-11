using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GroundCollision : MonoBehaviour, GenericCollision
{
    TerrainHeightController terrainHeightController;
    private void Start()
    {
        terrainHeightController = gameObject.GetComponent<TerrainHeightController>();
    }

    public void CollidedWithMissile(Collision collision, GameObject missile)
    {
        terrainHeightController.OnCollisionWithMissile(collision, missile);
    }
}
