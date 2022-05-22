#pragma once
#include "SDL.h"

//using main instead of SDL_Main
#undef main

#include "Game.h"
#include <vector>


#define MIN_WINDOW_WIDTH 400
#define MIN_WINDOW_HEIGHT 400

//defines for the default background color fo the window.
#define BACKGROUND_RED 122
#define BACKGROUND_GREEN 122
#define BACKGROUND_BLUE 122

enum WindowState
{
    STOP = 0, TITLE, CHESS
};
class Window
{
public:
    static SDL_Renderer* renderer;
	static int screenHeight;
	static int screenWidth;
	static WindowState windowState;

	Window();
	~Window();

	void init(const char* title, int xpos, int ypos, bool fullscreen);
	void update(float deltaTime);
	void clean();
	void titleScreen();
	void render();
	WindowState running();

	void handleEvents();
	void handleKeyDown(SDL_KeyboardEvent& key);
	void handleKeyUp(SDL_KeyboardEvent& key);

	void calculateInitialWindowDimensions();

private:

	SDL_Window* window;
	bool frozen;
	Game* game;

};

