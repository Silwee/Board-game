#pragma once
#include "SDL.h"
#include "SDL_image.h"
class Button
{

public:
	static void loadImages();
	static void destroyImages();

	static SDL_Texture* exitButtonTexture;
	static SDL_Texture* restartButtonTexture;
	static SDL_Texture* undoButtonTexture;
};


