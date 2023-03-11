using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class LevelSelection : MonoBehaviour
{
    public Button[] lvlButtons;

    void Start()
    {
        int levelAt = PlayerPrefs.GetInt("levelAt", 2);
        for (int i = 0; i < lvlButtons.Length; i++)
        {
            lvlButtons[i].interactable = false;
        }

        int levelsUnlocked = PlayerPrefs.GetInt("levelsUnlocked");
        if (levelsUnlocked == 0)
            levelsUnlocked = 1;
        for (int i = 0; i < levelsUnlocked; i++)
        {
            lvlButtons[i].interactable = true;
        }
    }

}
