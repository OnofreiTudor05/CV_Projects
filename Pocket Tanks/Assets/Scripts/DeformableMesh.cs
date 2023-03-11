using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class DeformableMesh : MonoBehaviour
{
    public List<Vector3> originalVertices;
    public List<Vector3> modifiedVertices;

    private MeshFilter plane;

    private void Start()
    {
        plane = GetComponent<MeshFilter>();
        plane.mesh.MarkDynamic();
        originalVertices = plane.mesh.vertices.ToList();
        modifiedVertices = plane.mesh.vertices.ToList();
    }

    public void AddDepression(Vector3 depressionPoint)
    {
        //Debug.Log("Collision point: " + depressionPoint);
        var worldPos4 = transform.worldToLocalMatrix * depressionPoint;
        var worldPos = new Vector3(worldPos4.x, worldPos4.y, worldPos4.z);

        for (int i = 0; i < modifiedVertices.Count; ++i)
        {
            var distance = (worldPos - (modifiedVertices[i] + Vector3.down)).magnitude;
            //Debug.Log(distance);
            if (distance < 1.5f)
            {
                //Debug.Log("Distance: " + distance);
                var newVert = originalVertices[i] + Vector3.down;
                modifiedVertices.RemoveAt(i);
                modifiedVertices.Insert(i, newVert);
            }
        }

        plane.mesh.SetVertices(modifiedVertices);
    }
}