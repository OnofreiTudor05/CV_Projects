using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour
{
    public void PlayGame()
    {
        PlayerPrefs.SetInt("levelsUnlocked", 1);
        SceneManager.LoadScene("SelectLevel");
    }

    public void Instructions()
    {
        SceneManager.LoadScene("Instructions");
    }

    public void Quit()
    {
        Application.Quit();
    }

    public void Back()
    {
        SceneManager.LoadScene("MainMenu");
    }

    public void Lvl1()
    {
        SceneManager.LoadScene("Level1");
    }

    public void Lvl2()
    {
        SceneManager.LoadScene("Level2");
    }

    public void Lvl3()
    {
        SceneManager.LoadScene("Level3");
    }

    public void Lvl4()
    {
        SceneManager.LoadScene("Level4");
    }

    public void Lvl5()
    {
        SceneManager.LoadScene("Level5");
    }

    public void Lvl6()
    {
        SceneManager.LoadScene("Level6");
    }

}
