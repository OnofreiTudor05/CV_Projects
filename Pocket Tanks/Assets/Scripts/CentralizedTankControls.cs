using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CentralizedTankControls : MonoBehaviour
{
    private GameObject currentlyPlayingTank;

    public void SetCurrentlyPlayingTank(GameObject tank)
    {
        currentlyPlayingTank = tank;
    }

    public void Shoot()
    {
        currentlyPlayingTank.GetComponent<TankController2>().Shoot();
    }
}
