using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface GenericCollision
{
    void CollidedWithMissile(Collision collision, GameObject missile);
}
