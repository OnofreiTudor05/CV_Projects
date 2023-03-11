using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ButtonMissile : MonoBehaviour
{
    public List<Button> buttons;
    public List<GameObject> missiles;

    public void Start()
    {
        // Set default
        // This is definitely not the best option, but we will find something better in the near future
        GameMissileHandler.currentGameMissile = missiles[0];

        /*Debug.Log(buttons.Count);
        for(int i = 0; i < buttons.Count; i++)
        {
            Debug.Log("Set listener for button " + buttons[i] + " and missile " + missiles[i]);
            buttons[i].onClick.AddListener(() =>
            {
                Debug.Log("New currentGameMissile: " + GameMissileHandler.currentGameMissile);
                int missilePosition = i;
                Debug.Log(missilePosition + " and i " + i);
                GameMissileHandler.currentGameMissile = missiles[missilePosition];
            });
        }*/
        buttons[0].onClick.AddListener(() =>
        {
            GameMissileHandler.currentGameMissile = missiles[0];
            Debug.Log("New currentGameMissile: " + GameMissileHandler.currentGameMissile);
        });

        buttons[1].onClick.AddListener(() =>
        {
            GameMissileHandler.currentGameMissile = missiles[1];
            Debug.Log("New currentGameMissile: " + GameMissileHandler.currentGameMissile);
        });

        buttons[2].onClick.AddListener(() =>
        {
            GameMissileHandler.currentGameMissile = missiles[2];
            Debug.Log("New currentGameMissile: " + GameMissileHandler.currentGameMissile);
        });

        buttons[3].onClick.AddListener(() =>
        {
            GameMissileHandler.currentGameMissile = missiles[3];
            Debug.Log("New currentGameMissile: " + GameMissileHandler.currentGameMissile);
        });
    }
}
