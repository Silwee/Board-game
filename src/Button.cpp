#include "Button.h"
#include "Window.h"

SDL_Texture* Button::exitButtonTexture;
SDL_Texture* Button::restartButtonTexture;
SDL_Texture* Button::undoButtonTexture;


void Button::loadImages()
{
    SDL_Surface* surface;

    surface = IMG_Load("images/exitButton.png");
    exitButtonTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);\
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/restartButton.png");
    restartButtonTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("images/undoButton.png");
    undoButtonTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_FreeSurface(surface);
}

void Button::destroyImages()
{
    SDL_DestroyTexture(exitButtonTexture);
    SDL_DestroyTexture(restartButtonTexture);
    SDL_DestroyTexture(undoButtonTexture);
}

