using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class OptionMenuScript : MonoBehaviour
{
    public void Play()
    {
        SceneManager.LoadScene("ARTests");
    }

    public void Options()
    {
        SceneManager.LoadScene("OptionsMenu");
    }

    public void Back()
    {
        SceneManager.LoadScene("MainMenu");
    }

    public void Rocket()
    {
        SceneManager.LoadScene("Rocket");
    }
    public void Ballistic()
    {
        SceneManager.LoadScene("Ballistic");
    }
    public void SplashySplash()
    {
        SceneManager.LoadScene("SplashySplash");
    }
    public void Pillow()
    {
        SceneManager.LoadScene("Pillow");
    }
    public void Bomb()
    {
        SceneManager.LoadScene("Bomb");
    }
    public void Quit()
    {
        Application.Quit();
    }
}