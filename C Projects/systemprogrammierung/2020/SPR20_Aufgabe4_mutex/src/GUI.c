/**
 * @file GUI.c
 * @brief GUI for distancing simulator. You dont have to read this file :)
 * @version 1.0
 * @date 2020-06-13
 * 
 */

#include <stdbool.h>
#include "SDL_image.h"

#include "GUI.h"
#include "Params.h"
#include "Utils.h"

#define FPS_SHOW_INTERVAL_SEC 1

struct GUI
{
	SDL_Window *window;
	SDL_Texture *texture_bg;
	SDL_Texture *texture_healthy;
	SDL_Texture *texture_sick;
	SDL_Event events;
	bool pause;
	bool quit;
	Uint32 timestamp_next_frame;
	int total_frame_count;
	int interval_frame_count;
	Uint32 timestamp_next_fps_update;
	Uint32 timestamp_begin;
	char *title;
};

/////////////////////////////////
// declaration local functions //
/////////////////////////////////

static void GUI_DrawState(GUI *gui, Person *state, int sim_num);

/////////////////////////////////
// definition global functions //
/////////////////////////////////

bool GUI_Update(GUI *gui, Person **state, int state_num)
{
	int i;

	// update graphics
	SDL_Renderer *renderer = SDL_GetRenderer(gui->window);
	SDL_RenderClear(renderer);

	for (i = 0; i < state_num; i++)
		GUI_DrawState(gui, state[i], i);

	SDL_RenderPresent(renderer);
	return true;
}

UserInput GUI_HandleUserInput(GUI *gui)
{
	// handle user input
	while (SDL_PollEvent(&gui->events)) {
		switch (gui->events.type)
		{
			case SDL_QUIT: return COMMAND_QUIT;
		}
	}
	return COMMAND_EMPTY;
}

void GUI_WaitNextFrame(GUI *gui)
{
	gui->interval_frame_count++;

	if (SDL_GetTicks() > gui->timestamp_next_fps_update)
	{
		gui->total_frame_count += gui->interval_frame_count;
		sprintf(gui->title, "Simulation %.2f FPS (last %d seconds: %2.f FPS)",
				(float) 1000 * gui->total_frame_count / (SDL_GetTicks() - gui->timestamp_begin),
				FPS_SHOW_INTERVAL_SEC,
				(float) 1000 * gui->interval_frame_count / (FPS_SHOW_INTERVAL_SEC * 1000)
			);
		SDL_SetWindowTitle(gui->window, gui->title);
		gui->interval_frame_count = 0;
		gui->timestamp_next_fps_update = SDL_GetTicks() + (FPS_SHOW_INTERVAL_SEC * 1000);
	}

	#if FPS_LIMIT
		// wait if updating too fast
		if (SDL_GetTicks() < gui->timestamp_next_frame)
		{
			SDL_Delay(gui->timestamp_next_frame - SDL_GetTicks());
		}
		gui->timestamp_next_frame += (1000 / FPS_LIMIT);
	#endif //FPS_LIMIT
}

GUI * GUI_Create(void)
{
	SDL_Renderer *renderer;

	GUI *gui = calloc(1, sizeof (struct GUI));

	gui->timestamp_next_fps_update = 0;

	gui->timestamp_begin = SDL_GetTicks();
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	gui->window = SDL_CreateWindow("Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GUI_WIDTH, GUI_HEIGHT, 0);

	if (NULL == gui->window)
		printf("Could not create window! SDL_Error: %s\n", SDL_GetError());

	renderer = SDL_CreateRenderer(gui->window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);

	// Prepare Textures
	SDL_Surface *bg = SDL_CreateRGBSurface(0, SCENARIO_WIDTH, SCENARIO_HEIGHT, 32, 0, 0, 0, 0);
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, 0xFF, 0xFF, 0xFF));
	gui->texture_bg = SDL_CreateTextureFromSurface(renderer, bg);
	SDL_FreeSurface(bg);

	SDL_Surface *healthy = IMG_Load("data/person_healthy.png");
	gui->texture_healthy = SDL_CreateTextureFromSurface(renderer, healthy);
	SDL_FreeSurface(healthy);

	SDL_Surface *sick = IMG_Load("data/person_sick.png");
	gui->texture_sick = SDL_CreateTextureFromSurface(renderer, sick);
	SDL_FreeSurface(sick);


	// calculate title length:
	gui->title = calloc(sizeof (char), sizeof ("Simulation  FPS (last %d seconds:  FPS)") + 2 * (log10(FPS_SHOW_INTERVAL_SEC) + 1 + 3));
	return gui;
}

void GUI_Destroy(GUI *gui)
{
	SDL_DestroyTexture(gui->texture_bg);
	SDL_DestroyTexture(gui->texture_healthy);
	SDL_DestroyTexture(gui->texture_sick);

	SDL_DestroyRenderer(SDL_GetRenderer(gui->window));
	SDL_DestroyWindow(gui->window);
	free(gui->title);
	free (gui);

	SDL_Quit();
}

////////////////////////////////
// definition local functions //
////////////////////////////////

static void GUI_DrawState(GUI *gui, Person *persons, int sim_num)
{
	SDL_Rect pos;
	SDL_Renderer *renderer = SDL_GetRenderer(gui->window);
	int offset_x = (sim_num * SCENARIO_WIDTH) + (2 * sim_num + 1) * GUI_BORDER_SIZE;
	int offset_y = 0;

	pos.x = offset_x;
	pos.y = offset_y;
	pos.w = SCENARIO_WIDTH;
	pos.h = SCENARIO_HEIGHT;

	SDL_RenderCopy(renderer, gui->texture_bg, NULL, &pos);

	pos.w = PERSON_SIZE;
	pos.h = PERSON_SIZE;

	for (int i = 0; i < PERSON_NUM; ++i)
	{
		pos.x = persons[i].pos.x - PERSON_SIZE/2 + offset_x;
		pos.y = persons[i].pos.y - PERSON_SIZE/2 + offset_y;

		if (persons[i].is_infected)
		{
			SDL_RenderCopy(renderer, gui->texture_sick, NULL, &pos);
		}
		else
		{
			SDL_RenderCopy(renderer, gui->texture_healthy, NULL, &pos);
		}
	}
}