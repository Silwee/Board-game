#include "Window.h"
#include "SDL_image.h"
#include <iostream>


int Window::screenHeight = 0;
int Window::screenWidth = 0;
SDL_Renderer* Window::renderer = nullptr;
WindowState Window::windowState = STOP;

Window::Window()
{
    frozen = true;
}

Window::~Window()
{

}

void Window::init(const char* title, int xpos, int ypos, bool fullscreen)
{
    int flags = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems Initialized!\n";

        //set window size depend on screen size
        calculateInitialWindowDimensions();

        window = SDL_CreateWindow(title, xpos, ypos, Window::screenWidth, Window::screenHeight, flags);
        if (window)
        {
            std::cout << "Window Created!\n";
            SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
            std::cout << "Renderer created!\n";
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        }
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            windowState = STOP;
        }
        else
        {
            windowState = TITLE;
            game = new Game();
            game->init();
        }
    }
    else
    {
        windowState = STOP;
        fprintf(stderr, "Something failed in SDL setup\n");
    }
}

void Window::update(float deltaTime)
{
    game->update();
}

void Window::titleScreen()
{
    SDL_Surface* surface;
    SDL_Texture* titleScreeenTexture;

    surface = IMG_Load("images/titleScreen.png");
    if (!surface)
    {
        std::cout << "Unable to load title image:" << IMG_GetError() << std::endl;
    }
    titleScreeenTexture = SDL_CreateTextureFromSurface(renderer, surface );
    if (!titleScreeenTexture)
    {
        std::cout << "Unable to create title texture:" << IMG_GetError() << std::endl;
    }
    SDL_RenderCopy(renderer, titleScreeenTexture, NULL, NULL );
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(titleScreeenTexture);
}

void Window::render()
{
    SDL_SetRenderDrawColor(renderer, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, 255);
    SDL_RenderClear(renderer);

    if (windowState == CHESS)
    {
        game->render();
    }

    SDL_RenderPresent(Window::renderer);
}


WindowState Window::running()
{
    return windowState;
}

void Window::clean()
{
    delete(game);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //std::cout << "Succesfully cleaned!\n";
}


void Window::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        windowState = STOP;
        break;
    case SDL_KEYDOWN:
        if (windowState == TITLE)
        {

        }
        else
        {
            handleKeyDown(event.key);
        }
        break;
    case SDL_KEYUP:
        if (windowState == TITLE)
        {

        }
        else
        {
            handleKeyUp(event.key);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (windowState == TITLE)
        {

        }
        else
        {
            game->handleMouseButtonDown(event.button);
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (windowState == TITLE)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (y*2 >= screenHeight && y*6 <= screenHeight*5)
            {
                if (x*8 >= screenWidth*3 && x*8 <= screenWidth*5)
                {
                    windowState = CHESS;
                }
            }
        }
        else
        {
            game->handleMouseButtonUp(event.button);
        }
        break;
    }
}

void Window::handleKeyDown(SDL_KeyboardEvent& key)
{
    switch (key.keysym.scancode)
    {
    case SDL_SCANCODE_R:
        game->reset();
        break;
    case SDL_SCANCODE_C:
        game->calculateBoardStates();
        break;
    case SDL_SCANCODE_Q:
        game->makeRandomMove();
        break;
    case SDL_SCANCODE_Z:
        game->unmakeMove();
        break;
    case SDL_SCANCODE_ESCAPE:
        windowState = TITLE;
        break;
    default:
        //std::cout << "Scancode is:" << key.keysym.scancode << std::endl;
        break;
    }
}
void Window::handleKeyUp(SDL_KeyboardEvent& key)
{
    switch (key.keysym.scancode)
    {
    default:
        //std::cout << "Scancode is:" << key.keysym.scancode << std::endl;
        break;
    }
}

//sets the initial height to be a square that is 80% of the smallest dimension.
void Window::calculateInitialWindowDimensions()
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h;

    int squareWidth;
    if (Width > Height)
    {
        squareWidth = .8 * Height;
    }
    else
    {
        squareWidth = .8 * Width;
    }
    Window::screenHeight = squareWidth;
    Window::screenWidth = squareWidth * 1920/float(1080);
}
