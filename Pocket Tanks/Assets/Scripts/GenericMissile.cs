using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "New missile", menuName = "Missile")]
public class GenericMissile : ScriptableObject
{
    private Vector3 initialPosition;
    private Quaternion initialRotation;

    // The name of the missile; Might be useful
    public string missileName;

    // The effect triggered upon hitting a target
    public GameObject explosionEffect;

    [Range(1f, 100f)]
    public float damage;

    [Range(0.1f, 5f)]
    public float radius = 2f;

    public Material material;

    // The mesh of the missile
    public Mesh mesh;

    public void InitializeObject(GameObject gameObject)
    {
        MeshFilter meshFilter = gameObject.AddComponent<MeshFilter>();
        // This is where we give the missile its specific look
        meshFilter.mesh = mesh;

        Renderer objectRenderer = gameObject.GetComponent<Renderer>();
        
        objectRenderer.material = material;

        initialPosition = gameObject.transform.position;
        initialRotation = gameObject.transform.rotation;
    }

    public void ResetMissilePosition(GameObject gameObject)
    {
        gameObject.transform.position = initialPosition;
        gameObject.transform.rotation = initialRotation;
        gameObject.GetComponentInChildren<Rigidbody>().velocity = Vector3.zero;
        gameObject.GetComponentInChildren<Rigidbody>().angularVelocity = Vector3.zero;
        gameObject.GetComponentInChildren<Rigidbody>().useGravity = false;
    }
}