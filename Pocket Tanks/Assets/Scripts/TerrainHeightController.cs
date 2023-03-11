using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TerrainHeightController : MonoBehaviour
{
    [Range(0.1f, 5f)]
    public float minimumVelocity = 2f;

    private Mesh mesh;

    private Vector3[] modifiedVertices;

    public void Start()
    {
        mesh = GetComponent<MeshFilter>().mesh;
        modifiedVertices = mesh.vertices;
    }

    public void OnCollisionWithMissile(Collision collision, GameObject missile)
    {
        Debug.Log("Missile collided with " + gameObject.name);
        if(collision.relativeVelocity.magnitude > minimumVelocity) {
            ChangeTerrainHeight(missile);
        }
    }

    public void ChangeTerrainHeight(GameObject missile)
    {
        for (int v = 0; v < modifiedVertices.Length; v++)
        {
            Vector3 distance = modifiedVertices[v] - (missile.transform.position - transform.position);
            float smoothingFactor = 2f;

            float force = 1f;// / (1f + missile.transform.position.sqrMagnitude);

            if (distance.sqrMagnitude < getMissileRadius(missile))
            {
                modifiedVertices[v] = modifiedVertices[v] + (Vector3.down * force) / smoothingFactor;
            }
        }
        RecalculateMesh();
    }

    public float getMissileDamage(GameObject missile)
    {
        return missile.GetComponent<MissileBehaviourManager>().genericMissile.damage;
    }
    public float getMissileRadius(GameObject missile)
    {
        return missile.GetComponent<MissileBehaviourManager>().genericMissile.radius;
    }

    void RecalculateMesh()
    {
        mesh.vertices = modifiedVertices;
        GetComponent<MeshCollider>().sharedMesh = mesh;
        mesh.RecalculateNormals();
    }

    private float GetMagnitudeDependantValue(float force, Collision collision)
    {
        // Requires testing
        return force * (collision.relativeVelocity.magnitude / 2);
    }
}
