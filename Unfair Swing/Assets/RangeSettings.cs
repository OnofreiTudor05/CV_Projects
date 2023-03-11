using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RangeSettings : MonoBehaviour
{

    [SerializeField]
    [Range(0, 1)]
    private float transparencyPercentage = 0.5f;

    [SerializeField]
    private float hookRange = 10f;
    void Start()
    {
        hookRange *= 2;
        hookRange += 2;
        setTransparency();
        setRadius();
    }

    void setTransparency()
    {
        SpriteRenderer spriteRenderer = GetComponent<SpriteRenderer>();
        spriteRenderer.color = new Color(
            spriteRenderer.color.r,
            spriteRenderer.color.g,
            spriteRenderer.color.b,
            transparencyPercentage
            );
    }

    void setRadius()
    {
        Transform transform = GetComponent<Transform>();

        transform.localScale = new Vector3(hookRange, hookRange, 0);
    }
}
