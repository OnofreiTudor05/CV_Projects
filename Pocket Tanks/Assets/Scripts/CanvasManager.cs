using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class CanvasManager : MonoBehaviour
{
    [SerializeField]
    private GameObject PlanePlacementUI;

    [SerializeField]
    private GameObject ControlsUI;

    [SerializeField]
    private GameObject GameOverUI;

    [SerializeField]
    private TextMeshProUGUI tank1HP;

    [SerializeField]
    private TextMeshProUGUI tank2HP;

    [SerializeField]
    private TextMeshProUGUI winner;

    public void GameOver(GameObject tank1, GameObject tank2, string winnerText)
    {
        SetUIStatus(ControlsUI, false);
        SetUIStatus(GameOverUI, true);
        SetEndingStats(tank1, tank2, winnerText);
    }

    public void ShowPlanePlacement()
    {
        SetUIStatus(PlanePlacementUI, true);
        SetUIStatus(ControlsUI, false);
        SetUIStatus(GameOverUI, false);
    }

    public void GameStart()
    {
        SetUIStatus(ControlsUI, true);
        SetUIStatus(PlanePlacementUI, false);
    }

    private void SetUIStatus(GameObject UI, bool status)
    {

        UI.SetActive(status);
    }

    private void SetEndingStats(GameObject tank1, GameObject tank2, string winnerText)
    {
        tank1HP.text = tank1.GetComponent<TankDamageReceiver>().HP.ToString();
        tank2HP.text = tank2.GetComponent<TankDamageReceiver>().HP.ToString();

        winner.text = winnerText;
    }
}
