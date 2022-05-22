#include "Game.h"
#include "Window.h"
int Game::boardTopLeftX;
int Game::boardTopLeftY;


Game::~Game()
{
    delete(board);
}

void Game::calculateBoardStates()
{
    board->calculateBoardStates();
}

void Game::render()
{
    board->render(board->getBoardState());
}

void Game::update()
{

}

void Game::handleMouseButtonDown(SDL_MouseButtonEvent& b)
{
    if (b.button == SDL_BUTTON_LEFT)
        board->handleMouseButtonDown(board->getBoardState());
}

void Game::handleMouseButtonUp(SDL_MouseButtonEvent& b)
{
    if (b.button == SDL_BUTTON_LEFT)
        board->handleMouseButtonUp(board->getBoardState());
}

void Game::init()
{
    boardTopLeftX = boardTopLeftY = 0;
    board = new Board();
    board->init();

    boardTopLeftX = (Window::screenWidth - board->getWidth()) / 2;
    boardTopLeftY = (Window::screenHeight - board->getHeight()) / 2;
}

void Game::reset()
{
    board->reset();
}

void Game::unmakeMove()
{
    board->unMakeMove(board->getBoardState());
}

void Game::makeRandomMove()
{
    board->makeRandomMove(board->getBoardState());
}
