#include "Chess_Board.h"
#include "Window.h"
#include "Button.h"

#define HIGHLIGHT_COLOR {0,255,0,50}
#define ATTACK_COLOR {255,0,0,100}
#define DANGER_COLOR {255,128,0,100}
#define DRAW_COLOR {0,215,215,200}
#define WIN_COLOR {255,215,0,200}
#define LAST_MOVE_COLOR {0,0,255,100}
#define AMOUNT_OF_BOX .95

void Board::render(BoardState* currentBoardState)
{
    renderBoard();
    renderOutsideBoard();

    if (draggingPiece)
    {
        renderHighlightMoves();
    }
    else if (moveStack.getSize() != 0)
    {
        renderPreviousMove();
    }
    if (highlightKingBox.x != -1 || winnerKing.x != -1 || drawFirstKing.x != -1)
    {
        renderKingBox();
    }

    //renderAttackedSquares();

    renderPieces(currentBoardState);
    if (draggingPiece)
    {
        renderDraggedPiece();
    }

    if (waitingForPromotionChoice)
    {
        renderPromotionOptions();
    }
}

void Board::renderBoard()
{
    for (int x = 0; x < boardXBoxes; ++x)
    {
        for (int y = 0; y < boardYBoxes; ++y)
        {
            SDL_Color currentColor = (x + y) % 2 == 0 ? boardColor1 : boardColor2;
            renderBox({ x,y }, currentColor);
        }
    }
}


void Board::renderOutsideBoard()
{
    SDL_Rect rect;
    SDL_Color color;
    color =  { 0,100,200,205 };
    SDL_SetRenderDrawColor (Window::renderer, color.r, color.g, color.b, color.a);

    rect.x = rect.y = 0;
    rect.w = Game::boardTopLeftX;
    rect.h = Window::screenHeight;
    SDL_RenderFillRect (Window::renderer, &rect);

    rect.x = Game::boardTopLeftX + getWidth();
    SDL_RenderFillRect (Window::renderer, &rect);

    //for button
    int buttonWidth = Game::boardTopLeftX / 2;
    int space = (getHeight() - (3*buttonWidth)) / float(4);
    SDL_Rect renderRect;
    renderRect.w = buttonWidth;
    renderRect.h = buttonWidth;
    renderRect.x = Game::boardTopLeftX / 4;

    renderRect.y = space;
    SDL_RenderCopy(Window::renderer, Button::exitButtonTexture, NULL, &renderRect);

    renderRect.y = 2*space + buttonWidth;
    SDL_RenderCopy(Window::renderer, Button::restartButtonTexture, NULL, &renderRect);

    renderRect.y = 3*space + 2*buttonWidth;
    SDL_RenderCopy(Window::renderer, Button::undoButtonTexture, NULL, &renderRect);
}

//we only want to render previous moves if we're not rendering a pieces
void Board::renderPreviousMove()
{
    StoreMove* prevMoveHistory = moveStack.getTop();
    Move prevMove = prevMoveHistory->getMove();

    renderBox(prevMove.fromBox, LAST_MOVE_COLOR);
    renderBox(prevMove.toBox, LAST_MOVE_COLOR);
}

void Board::renderBox(Box box, SDL_Color color)
{
    SDL_Rect highlightRect;
    highlightRect.w = boxXWidth;
    highlightRect.h = boxYHeight;
    SDL_SetRenderDrawColor(Window::renderer, color.r, color.g, color.b, color.a);

    highlightRect.x = Game::boardTopLeftX + box.x * boxXWidth;
    highlightRect.y = Game::boardTopLeftY + box.y * boxYHeight;
    SDL_RenderFillRect(Window::renderer, &highlightRect);

}

void Board::renderPromotionOptions()
{
    SDL_Rect renderRect;
    renderRect.w = boxXWidth * 1.5;
    renderRect.h = boxYHeight * 1.5;
    renderRect.x = Game::boardTopLeftX + getWidth() + (Game::boardTopLeftX - renderRect.w) / 2;
    renderRect.y = Game::boardTopLeftY + (getHeight() - renderRect.h * 4) / 2;
    int w, h;
    SDL_Rect fromRect;
    fromRect.x = fromRect.y = 0;
    if (boardState->getCurrentTurn() == 'w')
    {
        SDL_QueryTexture(Piece::whiteQueenTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::whiteQueenTexture, &fromRect, &renderRect);
        renderRect.y += renderRect.h;

        SDL_QueryTexture(Piece::whiteRookTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::whiteRookTexture, &fromRect, &renderRect);
        renderRect.y += renderRect.h;

        SDL_QueryTexture(Piece::whiteBishopTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::whiteBishopTexture, &fromRect, &renderRect);
        renderRect.y += renderRect.h;

        SDL_QueryTexture(Piece::whiteKnightTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::whiteKnightTexture, &fromRect, &renderRect);
    }
    else
    {
        SDL_QueryTexture(Piece::blackQueenTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::blackQueenTexture, &fromRect, &renderRect);
        renderRect.y += renderRect.h;

        SDL_QueryTexture(Piece::blackRookTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::blackRookTexture, &fromRect, &renderRect);
        renderRect.y += renderRect.h;

        SDL_QueryTexture(Piece::blackBishopTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::blackBishopTexture, &fromRect, &renderRect);
        renderRect.y += renderRect.h;

        SDL_QueryTexture(Piece::blackKnightTexture, NULL, NULL, &w, &h);
        fromRect.w = w;
        fromRect.h = h;
        SDL_RenderCopy(Window::renderer, Piece::blackKnightTexture, &fromRect, &renderRect);
    }
}

void Board::renderDraggedPiece()
{
    int w, h, mouseX, mouseY;
    SDL_QueryTexture(draggingPieceTexture, NULL, NULL, &w, &h);
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Rect fromRect, toRect;
    fromRect.w = w;
    fromRect.h = h;
    fromRect.x = fromRect.y = 0;

    toRect.w = boxXWidth * AMOUNT_OF_BOX;
    toRect.h = boxYHeight * AMOUNT_OF_BOX;



    toRect.x = mouseX - (toRect.w / 2);
    toRect.y = mouseY - (toRect.h / 2);


    SDL_RenderCopy(Window::renderer, draggingPieceTexture, &fromRect, &toRect);

}

void Board::renderAttackedSquares()
{
    for (int x = 0; x < boardXBoxes; x++)
    {
        for (int y = 0; y < boardYBoxes; y++)
        {
            if (squareAttacked({ x,y }, boardState))
            {
                renderBox({ x,y }, DANGER_COLOR);
            }
        }
    }
}


void Board::renderPieces(BoardState* currentBoardState)
{
    /*
    uint8_t** board = currentBoardState->getBoard();
    for (int x = 0; x < boardXBoxes; ++x) {
    	for (int y = 0; y < boardYBoxes; ++y) {
    		if (board[x][y] != 0) {

    			if (x != draggingPieceBox.x || y != draggingPieceBox.y) {
    				//std::cout << " rendering: " << int(board[x][y]) << std::endl;
    				renderPiece(x, y, currentBoardState);
    			}
    		}
    	}
    }*/
    if (draggingPiece)
    {
        if (currentBoardState->getBoard()[draggingPieceBox.x][draggingPieceBox.y] != (Piece::white | Piece::king))
        {
            renderPiece(whiteLocations.getKingLocation(), currentBoardState);
        }
        if (currentBoardState->getBoard()[draggingPieceBox.x][draggingPieceBox.y] != (Piece::black | Piece::king))
        {
            renderPiece(blackLocations.getKingLocation(), currentBoardState);
        }
    }
    else
    {
        renderPiece(whiteLocations.getKingLocation(), currentBoardState);
        renderPiece(blackLocations.getKingLocation(), currentBoardState);
    }


    for (unsigned i = 0; i < whiteLocations.getBishopLocations().size(); i++)
    {
        renderPiece(whiteLocations.getBishopLocations()[i], boardState);
    }
    for (unsigned i = 0; i < whiteLocations.getQueenLocations().size(); i++)
    {
        renderPiece(whiteLocations.getQueenLocations()[i], boardState);
    }
    for (unsigned i = 0; i < whiteLocations.getPawnLocations().size(); i++)
    {
        renderPiece(whiteLocations.getPawnLocations()[i], boardState);
    }
    for (unsigned i = 0; i < whiteLocations.getRookLocations().size(); i++)
    {
        renderPiece(whiteLocations.getRookLocations()[i], boardState);
    }
    for (unsigned i = 0; i < whiteLocations.getKnightLocations().size(); i++)
    {
        renderPiece(whiteLocations.getKnightLocations()[i], boardState);
    }
    for (unsigned i = 0; i < blackLocations.getBishopLocations().size(); i++)
    {
        renderPiece(blackLocations.getBishopLocations()[i], boardState);
    }
    for (unsigned i = 0; i < blackLocations.getQueenLocations().size(); i++)
    {
        renderPiece(blackLocations.getQueenLocations()[i], boardState);
    }
    for (unsigned i = 0; i < blackLocations.getPawnLocations().size(); i++)
    {
        renderPiece(blackLocations.getPawnLocations()[i], boardState);
    }
    for (unsigned i = 0; i < blackLocations.getRookLocations().size(); i++)
    {
        renderPiece(blackLocations.getRookLocations()[i], boardState);
    }
    for (unsigned i = 0; i < blackLocations.getKnightLocations().size(); i++)
    {
        renderPiece(blackLocations.getKnightLocations()[i], boardState);
    }

}

void Board::renderPiece(Box pieceBox, BoardState* currentBoardState)
{
    uint8_t currentPiece = currentBoardState->getBoard()[pieceBox.x][pieceBox.y];
    SDL_Texture* currentTexture = getTextureAtLocation(pieceBox.x, pieceBox.y, currentBoardState);
    renderPieceTexture(currentTexture, pieceBox.x, pieceBox.y);
}

SDL_Texture* Board::getTextureAtLocation(int x, int y, BoardState* currentBoardState)
{
    uint8_t currentPiece = currentBoardState->getBoard()[x][y];
    if (currentPiece == (Piece::black | Piece::king))
    {
        return Piece::blackKingTexture;
    }
    else if (currentPiece == (Piece::black | Piece::queen))
    {
        return Piece::blackQueenTexture;
    }
    else if (currentPiece == (Piece::black | Piece::bishop))
    {
        return Piece::blackBishopTexture;
    }
    else if (currentPiece == (Piece::black | Piece::knight))
    {
        return Piece::blackKnightTexture;
    }
    else if (currentPiece == (Piece::black | Piece::pawn))
    {
        return Piece::blackPawnTexture;
    }
    else if (currentPiece == (Piece::black | Piece::rook))
    {
        return Piece::blackRookTexture;
    }
    else if (currentPiece == (Piece::white | Piece::king))
    {
        return Piece::whiteKingTexture;
    }
    else if (currentPiece == (Piece::white | Piece::queen))
    {
        return Piece::whiteQueenTexture;
    }
    else if (currentPiece == (Piece::white | Piece::bishop))
    {
        return Piece::whiteBishopTexture;
    }
    else if (currentPiece == (Piece::white | Piece::knight))
    {
        return Piece::whiteKnightTexture;
    }
    else if (currentPiece == (Piece::white | Piece::pawn))
    {
        return Piece::whitePawnTexture;
    }
    else if (currentPiece == (Piece::white | Piece::rook))
    {
        return Piece::whiteRookTexture;
    }
}

void Board::renderPieceTexture(SDL_Texture* texture, int x, int y)
{
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect fromRect, toRect;
    fromRect.w = w;
    fromRect.h = h;
    fromRect.x = fromRect.y = 0;

    toRect.w = boxXWidth * AMOUNT_OF_BOX;
    toRect.h = boxYHeight * AMOUNT_OF_BOX;


    toRect.x = Game::boardTopLeftX + x * boxXWidth + (boxXWidth - toRect.w) / 2;
    toRect.y = Game::boardTopLeftY + y * boxYHeight + (boxYHeight - toRect.h) / 2;


    SDL_RenderCopy(Window::renderer, texture, &fromRect, &toRect);

}

void Board::renderKingBox()
{
    if (drawFirstKing.x != -1)
    {
        renderBox(drawFirstKing, DRAW_COLOR);
        renderBox(drawSecondKing, DRAW_COLOR);
        return;
    }
    if (highlightKingBox.x != -1)
    {
        renderBox(highlightKingBox, ATTACK_COLOR);
    }
    if (winnerKing.x != -1)
    {
        renderBox(winnerKing, WIN_COLOR);
    }
}

void Board::renderHighlightMoves()
{
    for (unsigned i = 0; i < highlightBoxes.size(); i++)
    {
        renderBox(highlightBoxes[i], HIGHLIGHT_COLOR);
    }
}

