#include "Window.h"
#include "SDL.h"
#include <iostream>
#include <ctime>

int main(int argc, char* argv[])
{
    srand(time(NULL));
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart = SDL_GetTicks();
    int frameTime = 0;
    Window* gameWindow = new Window;

    bool isTitleRendered = false;
    float deltaTime;

    gameWindow->init("Board-game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);

    while (gameWindow->running() != STOP)
    {
        deltaTime = SDL_GetTicks() - frameStart;
        frameStart = SDL_GetTicks();
        gameWindow->handleEvents();
        if (gameWindow->running() == TITLE)
        {
            if(!isTitleRendered)
            {
                gameWindow->titleScreen();
            }
            isTitleRendered = true;
        }
        else if (gameWindow->running() == CHESS)
        {
            isTitleRendered = false;
            gameWindow->update(float(deltaTime) / 1000.0);
            gameWindow->render();
        }
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    gameWindow->clean();
    delete gameWindow;

    return 0;
}
