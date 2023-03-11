using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MoveToNextLevel : MonoBehaviour
{
    public int iLevelToLoad;
    public string sLevelToLoad;

    public bool useIntegerToLoadLevel = false;

    void Start()
    {
        
    }

    public void OnTriggerEnter2D(Collider2D collision)
    {
        GameObject collisionGameObject = collision.gameObject;

        if (collisionGameObject.name == "BallTrigger")
        {
            LoadScene();
        }
    }

    void LoadScene()
    {
        PlayerPrefs.SetInt("levelsUnlocked", iLevelToLoad);
        Debug.Log(PlayerPrefs.GetInt("levelsUnlocked"));
        if (useIntegerToLoadLevel)
        {
            SceneManager.LoadScene(iLevelToLoad);
        }
        else
        {
            SceneManager.LoadScene(sLevelToLoad);
        }
    }

}
