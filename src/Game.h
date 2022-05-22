#pragma once
#include "SDL.h"
#include "Chess_Board.h"
class Game
{
private:

	Board* board;

public:

	static int boardTopLeftX;
	static int boardTopLeftY;

	~Game();

	void update();
	void render();

	void handleMouseButtonDown(SDL_MouseButtonEvent& b);
	void handleMouseButtonUp(SDL_MouseButtonEvent& b);
	void init();

	void calculateBoardStates();

	void reset();

	void unmakeMove();

	void makeRandomMove();


};

