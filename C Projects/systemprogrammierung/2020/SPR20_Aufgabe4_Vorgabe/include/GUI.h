/**
 * @file GUI.h
 * @brief GUI for distancing simulator. You dont have to read this file :)
 * @version 1.0
 * @date 2020-06-13
 * 
 */

#include <SDL.h>
#include "Scenario.h"

typedef struct GUI GUI;

bool GUI_Update(GUI *window, Person **state, int state_num);

GUI * GUI_Create(void);

void GUI_Destroy(GUI *window);

UserInput GUI_HandleUserInput(GUI *gui);

void GUI_WaitNextFrame(GUI *gui);
