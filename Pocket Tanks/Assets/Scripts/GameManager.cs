using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.ARFoundation.Samples;
using UnityEngine.XR.ARFoundation;

public class GameManager : MonoBehaviour
{
    public GameObject gameScene;

    public GameObject tank1;

    public GameObject tank2;

    public GameObject ARSessionOrigin;

    public GameObject menuTank1;

    public GameObject menuTank2;

    [SerializeField]
    private int numberOfAbilities = 4;

    [SerializeField]
    private int shotAbilities = 0;

    public void Awake()
    {
        InitializeGame();
    }

    public void EndTurn()
    {
        shotAbilities++;
        if (shotAbilities < numberOfAbilities)
        {
            ChangeTurn();
        }
        else
        {
            EndGame();
        }
    }

    private void ChangeTurn()
    {
        if (GetTankControlStatus(tank1))
        {
            SetTankControlStatus(tank1, menuTank1, false);
            SetTankControlStatus(tank2, menuTank2, true);
            ResetMovement(tank2);
        }
        else
        {
            SetTankControlStatus(tank2, menuTank2, false);
            SetTankControlStatus(tank1, menuTank1, true);
            ResetMovement(tank1);
        }
    }

    private void EndGame()
    {
        Debug.Log("Game finished");

        DisableBothTanks();

        var tank1HP = tank1.GetComponent<TankDamageReceiver>().HP;
        var tank2HP = tank2.GetComponent<TankDamageReceiver>().HP;
        Debug.Log("Tank 1 HP: " + tank1HP);
        Debug.Log("Tank 2 HP: " + tank2HP);

        if(tank1HP > tank2HP)
        {
            Debug.Log("Tank1 won");
            GameOver("Tank1");
        } else if(tank2HP > tank1HP)
        {
            Debug.Log("Tank2 won");
            GameOver("Tank2");
        } else
        {
            Debug.Log("Draw");
            GameOver("Draw");
        }
    }

    private void DisableBothTanks()
    {
        SetTankControlStatus(tank1, menuTank1, false);
        SetTankControlStatus(tank2, menuTank2, false);
    }

    private void SetTankControlStatus(GameObject tank, GameObject tankMenu, bool activeStatus)
    {
        tank.GetComponentInChildren<TankController2>().enabled = activeStatus;
        if(activeStatus)
        {
            GetComponent<CentralizedTankControls>().SetCurrentlyPlayingTank(tank);
        }

        tankMenu.SetActive(activeStatus);
    }

    private bool GetTankControlStatus(GameObject tank)
    {
        return tank.GetComponentInChildren<TankController2>().enabled;
    }

    private void InitializeGame()
    {
        // Disable the second tank's controls
        SetTankControlStatus(tank2, menuTank2, false);

        // Enable the first tank's controls
        SetTankControlStatus(tank1, menuTank1, true);

        // Enable the ControlsUI and disable the GameOverUI
        GetComponent<CanvasManager>().ShowPlanePlacement();

        // Enable if they were disabled last instance of the game
        /*ARSessionOrigin.GetComponent<PlaceOnPlane>().enabled = true;
        ARSessionOrigin.GetComponent<ARPlaneManager>().requestedDetectionMode =
            (UnityEngine.XR.ARSubsystems.PlaneDetectionMode)(-1);*/
    }

    public void GameStart()
    {
        //gameScene.transform.position += new Vector3(-1000, 0, 0);

        GetComponent<CanvasManager>().GameStart();
        ARSessionOrigin.GetComponent<PlaceOnPlane>().enabled = false;
        ARSessionOrigin.GetComponent<ARPlaneManager>().requestedDetectionMode =
            UnityEngine.XR.ARSubsystems.PlaneDetectionMode.None;
    }

    private void GameOver(string winnerText)
    {
        GetComponent<CanvasManager>().GameOver(tank1, tank2, winnerText);
    }

    private void ResetMovement(GameObject tank1)
    {
        tank1.GetComponent<TankController2>().currentMovement = 0f;
    }

}
