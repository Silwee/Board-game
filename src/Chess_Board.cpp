#include "Chess_Board.h"
#include <iostream>
#include "Window.h"
#include <cctype>
#include "Button.h"

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TEST_FEN_1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define TEST_FEN_2 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
#define TEST_FEN_3 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq 0 1"
#define TEST_FEN_4 "7k/Q7/7K/8/8/8/8/8 w - - 1 8"
#define TEST_FEN_5 "rnbqkbn1/pppppppp/7r/8/8/P7/PPPP1PPP/RNBQKBNR w KQq - 0 1"

int Board::boxXWidth;
int Board::boxYHeight;
int Board::boardXBoxes;
int Board::boardYBoxes;


void Board::init()
{
    Piece::init();
    Button::loadImages();
    boardXBoxes = 8;
    boardYBoxes = 8;
    boxXWidth = Window::screenHeight / boardXBoxes;
    boxYHeight = Window::screenHeight / boardYBoxes;

    boardColor1 = { 234,233,210,255 };

    boardColor2 = { 75,115,153,255 };

    boardState = new BoardState();

    uint8_t** board = new uint8_t * [boardXBoxes];
    for (int i = 0; i < boardXBoxes; ++i)
    {
        board[i] = new uint8_t[boardYBoxes];
    }
    for (int x = 0; x < boardXBoxes; ++x)
    {
        for (int y = 0; y < boardYBoxes; ++y)
        {
            board[x][y] = 0;
        }
    }
    boardState->setBoard(board);

    draggingPiece = false;
    draggingPieceBox = {-1,-1};
    loadBoardFromFen(STARTING_FEN, boardState);
    initializePieceLocations(boardState);
    initializeKingsThreatened(boardState);

    legalMoves = calculateLegalMoves(boardState);

    highlightKingBox = {-1,-1};
    winnerKing = {-1,-1};
    drawFirstKing = {-1,-1};
    drawSecondKing = {-1,-1};
    gameState = RUNNING;
    moveStack.init();
}

void Board::reset()
{
    for (int x = 0; x < boardXBoxes; ++x)
    {
        for (int y = 0; y < boardYBoxes; ++y)
        {
            boardState->getBoard()[x][y] = 0;
        }
    }
    draggingPiece = false;
    draggingPieceBox = {-1,-1};
    highlightKingBox = {-1,-1};
    winnerKing = {-1,-1};
    drawFirstKing = {-1,-1};
    drawSecondKing = {-1,-1};
    gameState = RUNNING;

    loadBoardFromFen(STARTING_FEN, boardState);
    initializePieceLocations(boardState);
    clearThreats();
    moveStack.clear();

    legalMoves = calculateLegalMoves(boardState);
}

void Board::update()
{

}

Board::~Board()
{
    Piece::destroyImages();
    SDL_DestroyTexture(draggingPieceTexture);
    delete(boardState);
    moveStack.clear();
}

void Board::initializePieceLocations(BoardState* currentBoardState)
{
    blackLocations.clear();
    whiteLocations.clear();
    for (int x = 0; x < boardXBoxes; x++)
    {
        for (int y = 0; y < boardYBoxes; y++)
        {
            if (currentBoardState->getBoard()[x][y] != 0)
            {
                uint8_t currentPiece = currentBoardState->getBoard()[x][y];

                if (currentPiece == (Piece::black | Piece::king))
                {
                    blackLocations.setKingLocation({ x,y });
                }
                else if (currentPiece == (Piece::black | Piece::queen))
                {
                    blackLocations.getQueenLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::black | Piece::bishop))
                {
                    blackLocations.getBishopLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::black | Piece::knight))
                {
                    blackLocations.getKnightLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::black | Piece::pawn))
                {
                    blackLocations.getPawnLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::black | Piece::rook))
                {
                    blackLocations.getRookLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::white | Piece::king))
                {
                    whiteLocations.setKingLocation({ x,y });
                }
                else if (currentPiece == (Piece::white | Piece::queen))
                {
                    whiteLocations.getQueenLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::white | Piece::bishop))
                {
                    whiteLocations.getBishopLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::white | Piece::knight))
                {
                    whiteLocations.getKnightLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::white | Piece::pawn))
                {
                    whiteLocations.getPawnLocations().push_back({ x,y });
                }
                else if (currentPiece == (Piece::white | Piece::rook))
                {
                    whiteLocations.getRookLocations().push_back({ x,y });
                }
            }
        }
    }
}

void Board::loadBoardFromFen(const char* fenNotation, BoardState* currentBoardState)
{
    int index = 0;
    int column;

    uint8_t** board = currentBoardState->getBoard();
    for (int rank = 0; rank < boardYBoxes; rank++)
    {
        column = 0;
        while (fenNotation[index] != '/' && fenNotation[index] != ' ')
        {
            if (isdigit(fenNotation[index]))
            {
                column += (fenNotation[index] - '0');
                index++;
            }
            else
            {
                switch (fenNotation[index])
                {
                case 'P':
                    board[column][rank] = Piece::white | Piece::pawn;
                    break;
                case 'p':
                    board[column][rank] = Piece::black | Piece::pawn;
                    break;
                case 'R':
                    board[column][rank] = Piece::white | Piece::rook;
                    break;
                case 'r':
                    board[column][rank] = Piece::black | Piece::rook;
                    break;
                case 'N':
                    board[column][rank] = Piece::white | Piece::knight;
                    break;
                case 'n':
                    board[column][rank] = Piece::black | Piece::knight;
                    break;
                case 'B':
                    board[column][rank] = Piece::white | Piece::bishop;
                    break;
                case 'b':
                    board[column][rank] = Piece::black | Piece::bishop;
                    break;
                case 'Q':
                    board[column][rank] = Piece::white | Piece::queen;
                    break;
                case 'q':
                    board[column][rank] = Piece::black | Piece::queen;
                    break;
                case 'K':
                    board[column][rank] = Piece::white | Piece::king;
                    break;
                case 'k':
                    board[column][rank] = Piece::black | Piece::king;
                    break;
                }
                ++index;
                ++column;
            }
        }
        ++index;
    }
    //now we are out of the long /'s, and are onto the current players turn.
    currentBoardState->setCurrentTurn(fenNotation[index]);
    index += 2;
    currentBoardState->setWhiteCanKingsideCastle(false);
    currentBoardState->setBlackCanKingsideCastle(false);
    currentBoardState->setWhiteCanQueensideCastle(false);
    currentBoardState->setBlackCanQueensideCastle(false);

    //now we are onto the castling.
    if (fenNotation[index] == '-')
    {
        index += 2;
    }
    else
    {
        while (fenNotation[index] != ' ')
        {
            switch (fenNotation[index])
            {
            case 'K':
                currentBoardState->setWhiteCanKingsideCastle(true);
                break;
            case 'k':
                currentBoardState->setBlackCanKingsideCastle(true);
                break;
            case 'Q':
                currentBoardState->setWhiteCanQueensideCastle(true);
                break;
            case 'q':
                currentBoardState->setBlackCanQueensideCastle(true);
                break;
            }
            ++index;
        }
        ++index;
    }

    //now we are onto the en-passant option.

    if (fenNotation[index] == '-')
    {
        index += 2;
        currentBoardState->setEnPassantX(-1);
        currentBoardState->setEnPassantY(-1);
    }
    else
    {
        currentBoardState->setEnPassantX(fenNotation[index] - 'a');
        ++index;
        currentBoardState->setEnPassantY(boardYBoxes - (fenNotation[index] - '0'));
        index += 2;
    }

    currentBoardState->setHalfMoveClock(fenNotation[index] - '0');
    ++index;
    while (fenNotation[index] != ' ')
    {
        currentBoardState->setHalfMoveClock(currentBoardState->getHalfMoveClock() * 10 + int(fenNotation[index] - '0'));
        ++index;
    }

    ++index;
    currentBoardState->setFullMoveClock(fenNotation[index] - '0');
    ++index;
    while (fenNotation[index] != '\0' && fenNotation[index] != ' ')
    {
        currentBoardState->setFullMoveClock(currentBoardState->getFullMoveClock() * 10 + int(fenNotation[index] - '0'));
        ++index;
    }
}


void Board::handleMouseButtonDown(BoardState* currentBoardState)
{
    int x, y, boardX, boardY;
    SDL_GetMouseState(&x, &y);
    if (x < Game::boardTopLeftX || x > Game::boardTopLeftX + getWidth())
    {

    }
    else if (y < Game::boardTopLeftY || y > Game::boardTopLeftY + getHeight())
    {

    }
    else
    {
        boardX = (x - Game::boardTopLeftX) / boxXWidth;
        boardY = (y - Game::boardTopLeftY) / boxYHeight;
        if (!waitingForPromotionChoice && gameState == RUNNING)
        {
            attemptPickupPiece({boardX, boardY}, currentBoardState);
        }
    }
}

void Board::handleMouseButtonUp(BoardState* currentBoardState)
{
    int x, y, boardX, boardY;
    SDL_GetMouseState(&x, &y);
    if (x < Game::boardTopLeftX || y < Game::boardTopLeftY)
    {
        if(x*4 >= Game::boardTopLeftX && x*4 <= Game::boardTopLeftX*3)
        {
            int buttonWidth = Game::boardTopLeftX / 2;
            int space = (getHeight() - (3*buttonWidth)) / float(4);
            if(y >= space && y <= space+buttonWidth)
            {
                reset();
                Window::windowState = TITLE;
            }
            else if(y >= 2*space+buttonWidth && y <= 2*space+2*buttonWidth)
            {
                reset();
            }
            else if(y >= 3*space+2*buttonWidth && y <= 3*space+3*buttonWidth)
            {
                unMakeMove(currentBoardState);
            }
        }
    }
    else if (x > Game::boardTopLeftX + getWidth() || y > Game::boardTopLeftY + getHeight())
    {
        if (waitingForPromotionChoice)
        {
            int promotionOpionWidth = boxXWidth * 1.5;
            int xSpacePromotion = (Game::boardTopLeftX - promotionOpionWidth) / 2;
            int ySpacePromotion = Game::boardTopLeftY + (getHeight() - promotionOpionWidth * 4) / 2;
            x -= Game::boardTopLeftX + getWidth();
            y -= ySpacePromotion;
            if (x >= xSpacePromotion && x <= xSpacePromotion + promotionOpionWidth)
            {
                tryPickingPromotionPiece(y / promotionOpionWidth, currentBoardState);
            }
        }
    }
    else
    {
        boardX = (x - Game::boardTopLeftX) / boxXWidth;
        boardY = (y - Game::boardTopLeftY) / boxYHeight;
        if (!waitingForPromotionChoice && draggingPiece && gameState == RUNNING)
        {
            attemptPlacePiece({boardX, boardY}, currentBoardState);
        }
    }
}

void Board::attemptPickupPiece(Box currentBox, BoardState* currentBoardState)
{
    //if there's a piece on the board space we're clicking.
    if (currentBoardState->getBoard()[currentBox.x][currentBox.y] != 0)
    {
        //if it's the piece for the curernt players turn.
        if (pieceIsCurrentPlayersPiece(currentBox.x, currentBox.y, currentBoardState))
        {
            draggingPiece = true;
            draggingPieceBox = currentBox;
            draggingPieceTexture = getTextureAtLocation(currentBox.x, currentBox.y, currentBoardState);
            createHighlightMoves(currentBox);
            if (currentBoardState->getCurrentTurn() == 'w')
            {
                whiteLocations.removePiece(currentBoardState->getBoard()[currentBox.x][currentBox.y], currentBox);
            }
            else
            {
                blackLocations.removePiece(currentBoardState->getBoard()[currentBox.x][currentBox.y], currentBox);
            }
        }
    }
}

void Board::attemptPlacePiece(Box currentBox, BoardState* currentBoardState)
{
    Move newMove = { draggingPieceBox.x,draggingPieceBox.y, currentBox.x, currentBox.y };
    uint8_t** board = currentBoardState->getBoard();

    //inLegalMoves passes in a reference so we update the castling and stuff here.
    if (inLegalMoves(newMove))
    {
        if (currentBoardState->getCurrentTurn() == 'w')
        {
            whiteLocations.addPiece(board[draggingPieceBox.x][draggingPieceBox.y], draggingPieceBox);
        }
        else
        {
            blackLocations.addPiece(board[draggingPieceBox.x][draggingPieceBox.y], draggingPieceBox);
        }

        if (newMove.isPromotion)
        {
            promotionMove =
            {
                newMove.fromBox,
                newMove.toBox,
                false,
                false,
                false,
                true,
                newMove.promotionType
            };
            waitingForPromotionChoice = true;
        }
        else
        {
            makeMove(newMove, currentBoardState);
            stopDraggingPiece();
            nextTurn(currentBoardState);
        }
        stopDraggingPiece();
    }
    else
    {
        if (currentBoardState->getCurrentTurn() == 'w')
        {
            whiteLocations.addPiece(board[draggingPieceBox.x][draggingPieceBox.y], draggingPieceBox);
        }
        else
        {
            blackLocations.addPiece(board[draggingPieceBox.x][draggingPieceBox.y], draggingPieceBox);
        }
        stopDraggingPiece();
    }

    //printBoardState(currentBoardState);
}

void Board::stopDraggingPiece()
{
    draggingPiece = false;
    draggingPieceBox = {-1,-1};
}


void Board::tryPickingPromotionPiece(int option, BoardState* currentBoardState)
{
    switch (option)
    {
    case 0:
        promotePiece(currentBoardState,'q');
        break;
    case 1:
        promotePiece(currentBoardState,'r');
        break;
    case 2:
        promotePiece(currentBoardState,'b');
        break;
    case 3:
        promotePiece(currentBoardState,'n');
        break;
    }
    waitingForPromotionChoice = false;
    nextTurn(currentBoardState);
}

void Board::promotePiece(BoardState* currentBoardState, char type)
{
    makeMove({ promotionMove.fromBox, promotionMove.toBox,
               false,false,false,true,type }, currentBoardState);
}

void Board::nextTurn(BoardState* currentBoardState)
{
    if (draggingPiece) return;
    if (!moveStack.getSize()) return;
    legalMoves = calculateLegalMoves(currentBoardState);

    int temp = isGameOver(currentBoardState);
    if (temp == 1)
    {
        gameState = LOST;
    }
    else if (temp == 2)
    {
        gameState = DRAW;
    }
    else
    {
        //increase the full move counter every 2 moves
        if (currentBoardState->getCurrentTurn() == 'w')
        {
            currentBoardState->setFullMoveClock(currentBoardState->getFullMoveClock() + 1);
        }

        //erase the half move counter after a capture or a pawn move, increase it otherwise
        StoreMove* lastMove = moveStack.getTop();
        if (lastMove->tookPiece() || lastMove->getPawnMove())
        {
            currentBoardState->setHalfMoveClock(0);
        }
        else
        {
            currentBoardState->setHalfMoveClock(currentBoardState->getHalfMoveClock() + 1);
        }
    }
    updateHighlightKingBox();
}

void Board::switchTurns(BoardState* currentBoardState)
{
    currentBoardState->setCurrentTurn((currentBoardState->getCurrentTurn() == 'w') ? 'b' : 'w');
}

void Board::clearMoves()
{
    legalMoves.clear();
}

void Board::makeMove(Move move, BoardState* currentBoardState)
{
    StoreMove* newStore = new StoreMove();

    uint8_t** board = currentBoardState->getBoard();
    int enPassantX = currentBoardState->getEnPassantX();
    int enPassantY = currentBoardState->getEnPassantY();
    char turn = currentBoardState->getCurrentTurn();
    newStore->setPreviousEnPassant({ enPassantX,enPassantY });
    if (isEnPassant(move.fromBox, move.toBox, currentBoardState))
    {
        //the piece removed depends on the turn.
        if (turn == 'w')
        {
            board[enPassantX][enPassantY + 1] = 0;
            blackLocations.removePawn({ enPassantX,enPassantY + 1 });
        }
        else
        {
            board[enPassantX][enPassantY - 1] = 0;
            whiteLocations.removePawn({ enPassantX,enPassantY - 1 });
        }
        newStore->setPawnMove(true);
    }

    if (move.kingSideCastle)
    {
        board[move.toBox.x - 1][move.toBox.y] = board[move.toBox.x + 1][move.toBox.y];
        board[move.toBox.x + 1][move.toBox.y] = 0;
        if (turn == 'w')
        {
            whiteLocations.updateRook({ move.toBox.x + 1,move.toBox.y }, { move.toBox.x - 1,move.toBox.y });
        }
        else
        {
            blackLocations.updateRook({ move.toBox.x + 1,move.toBox.y }, { move.toBox.x - 1,move.toBox.y });
        }

    }
    else if (move.queenSideCastle)
    {
        board[move.toBox.x + 1][move.toBox.y] = board[move.toBox.x - 2][move.toBox.y];
        board[move.toBox.x - 2][move.toBox.y] = 0;

        if (turn == 'w')
        {
            whiteLocations.updateRook({ move.toBox.x - 2,move.toBox.y }, { move.toBox.x + 1,move.toBox.y });
        }
        else
        {
            blackLocations.updateRook({ move.toBox.x - 2,move.toBox.y }, { move.toBox.x + 1,move.toBox.y });
        }
    }

    updateEnPassant(move.fromBox, move.toBox, currentBoardState);
    newStore->setPreviousCastles(currentBoardState->getWhiteCanKingsideCastle(),
                                 currentBoardState->getWhiteCanQueensideCastle(),
                                 currentBoardState->getBlackCanKingsideCastle(),
                                 currentBoardState->getBlackCanQueensideCastle());
    updateCastling(move.fromBox, move.toBox, currentBoardState);



    if (board[move.toBox.x][move.toBox.y] != 0)
    {
        newStore->setPieceTaken(board[move.toBox.x][move.toBox.y]);
        newStore->setTookPiece(true);
        if (turn == 'w')
        {
            blackLocations.removePiece(board[move.toBox.x][move.toBox.y], { move.toBox });
        }
        else
        {
            whiteLocations.removePiece(board[move.toBox.x][move.toBox.y], { move.toBox });
        }
    }
    else
    {
        newStore->setTookPiece(false);
    }
    if (move.isPromotion)
    {
        newStore->setPawnMove(true);
        switch (move.promotionType)
        {
        case 'q':
            if (turn == 'w')
            {
                board[move.toBox.x][move.toBox.y] = Piece::white | Piece::queen;
                whiteLocations.getQueenLocations().push_back( move.toBox );
                whiteLocations.removePawn(move.fromBox);
            }
            else
            {
                board[move.toBox.x][move.toBox.y] = Piece::black | Piece::queen;
                blackLocations.getQueenLocations().push_back( move.toBox );
                blackLocations.removePawn(move.fromBox);
            }
            break;
        case 'r':
            if (turn == 'w')
            {
                board[move.toBox.x][move.toBox.y] = Piece::white | Piece::rook;
                whiteLocations.getRookLocations().push_back( move.toBox );
                whiteLocations.removePawn(move.fromBox);
            }
            else
            {
                board[move.toBox.x][move.toBox.y] = Piece::black | Piece::rook;
                blackLocations.getRookLocations().push_back( move.toBox );
                blackLocations.removePawn(move.fromBox);
            }
            break;
        case 'n':
            if (turn == 'w')
            {
                board[move.toBox.x][move.toBox.y] = Piece::white | Piece::knight;
                whiteLocations.getKnightLocations().push_back( move.toBox );
                whiteLocations.removePawn(move.fromBox);
            }
            else
            {
                board[move.toBox.x][move.toBox.y] = Piece::black | Piece::knight;
                blackLocations.getKnightLocations().push_back( move.toBox );
                blackLocations.removePawn(move.fromBox);
            }
            break;

        case 'b':
            if (turn == 'w')
            {
                board[move.toBox.x][move.toBox.y] = Piece::white | Piece::bishop;
                whiteLocations.getBishopLocations().push_back( move.toBox );
                whiteLocations.removePawn(move.fromBox);
            }
            else
            {
                board[move.toBox.x][move.toBox.y] = Piece::black | Piece::bishop;
                blackLocations.getBishopLocations().push_back( move.toBox );
                blackLocations.removePawn(move.fromBox);
            }
            break;
        }
    }
    else
    {
        if((board[move.fromBox.x][move.fromBox.y] & Piece::pawn) == Piece::pawn)
        {
            newStore->setPawnMove(true);
        }
        else
        {
            newStore->setPawnMove(false);
        }
        board[move.toBox.x][move.toBox.y] = board[move.fromBox.x][move.fromBox.y];
        if (turn == 'w')
        {
            whiteLocations.updatePiece(board[move.toBox.x][move.toBox.y], move.fromBox, move.toBox);
        }
        else
        {
            blackLocations.updatePiece(board[move.toBox.x][move.toBox.y], move.fromBox, move.toBox);
        }
    }

    board[move.fromBox.x][move.fromBox.y] = 0;

    newStore->setMove(move);

    if (turn == 'w')
    {
        newStore->setThreatInfo(whiteThreatened.threatenedInfo, blackThreatened.threatenedInfo,
                                whiteThreatened.attackedInfo, blackThreatened.attackedInfo, whiteThreatened.amountAttacked,whiteThreatened.attackedByKnight,whiteThreatened.attackedFromBox);
    }
    else
    {
        newStore->setThreatInfo(whiteThreatened.threatenedInfo, blackThreatened.threatenedInfo,
                                whiteThreatened.attackedInfo, blackThreatened.attackedInfo, blackThreatened.amountAttacked,blackThreatened.attackedByKnight,blackThreatened.attackedFromBox);
    }
    newStore->setThreatBoxes(whiteThreatened.straightLeftBox, whiteThreatened.upLeftBox, whiteThreatened.straightUpBox,
                             whiteThreatened.upRightBox, whiteThreatened.straightRightBox, whiteThreatened.downRightBox, whiteThreatened.straightDownBox,
                             whiteThreatened.downLeftBox,

                             blackThreatened.straightLeftBox, blackThreatened.upLeftBox, blackThreatened.straightUpBox,
                             blackThreatened.upRightBox, blackThreatened.straightRightBox, blackThreatened.downRightBox, blackThreatened.straightDownBox,
                             blackThreatened.downLeftBox
                            );

    newStore->setPreviousHalfMoveClock(currentBoardState->getHalfMoveClock());

    moveStack.push(newStore);

    std::cout <<  char('a' + move.fromBox.x) << 8 - move.fromBox.y  << char('a' + move.toBox.x) << 8 - move.toBox.y << std::endl;


    //clear attack info because by virtue of how moves work, after a move you shouldn't be in check anymore.
    if (turn == 'w')
    {
        whiteThreatened.attackedInfo = 0;
        whiteThreatened.amountAttacked = 0;
    }
    else
    {
        blackThreatened.attackedInfo = 0;
        blackThreatened.amountAttacked = 0;
    }

    //if we moved to a thing related to the king we might be removing a threat.
    updateThreats(move, currentBoardState);

    switchTurns(currentBoardState);

    updateThreats(move, currentBoardState);

}


void Board::unMakeMove(BoardState* currentBoardState)
{
    if (draggingPiece) return;

    if (moveStack.getSize() == 0)
    {
        std::cout << "There are no moves to unmake" << std::endl;
        return;
    }

    if (gameState)
    {
        std::cout << "The game is over!" << std::endl;
        return;
    }

    switchTurns(currentBoardState);

    uint8_t** board = currentBoardState->getBoard();
    StoreMove* previousMove = moveStack.pop();
    Move move = previousMove->getMove();
    char currentTurn = currentBoardState->getCurrentTurn();
    //if it's a promotion we move the piece back and transform it into a pawn.
    if (move.isPromotion)
    {
        if (currentTurn == 'w')
        {
            board[move.fromBox.x][move.fromBox.y] = Piece::white | Piece::pawn;
            whiteLocations.removePiece(board[move.toBox.x][move.toBox.y], move.toBox);
            whiteLocations.getPawnLocations().push_back(move.fromBox);
        }
        else
        {
            board[move.fromBox.x][move.fromBox.y] = Piece::black | Piece::pawn;
            blackLocations.removePiece(board[move.toBox.x][move.toBox.y], move.toBox);
            blackLocations.getPawnLocations().push_back(move.fromBox);
        }

    }
    //otherwise we can just put the piece back normally
    else
    {
        board[move.fromBox.x][move.fromBox.y] = board[move.toBox.x][move.toBox.y];
        if (currentTurn == 'w')
        {
            whiteLocations.updatePiece(board[move.fromBox.x][move.fromBox.y], move.toBox, move.fromBox);
        }
        else
        {
            blackLocations.updatePiece(board[move.fromBox.x][move.fromBox.y], move.toBox, move.fromBox);
        }
    }
    //return the piece that was taken
    if (previousMove->tookPiece())
    {
        board[move.toBox.x][move.toBox.y] = previousMove->getTakenPiece();
        if (currentTurn == 'w')
        {
            blackLocations.addPiece(previousMove->getTakenPiece(), move.toBox);
        }
        else
        {
            whiteLocations.addPiece(previousMove->getTakenPiece(), move.toBox);
        }
    }
    else
    {
        board[move.toBox.x][move.toBox.y] = 0;
    }

    currentBoardState->setBlackCanKingsideCastle(previousMove->getPreviousBlackKingSide());
    currentBoardState->setWhiteCanKingsideCastle(previousMove->getPreviousWhiteKingSide());
    currentBoardState->setWhiteCanQueensideCastle(previousMove->getPreviousWhiteQueenSide());
    currentBoardState->setBlackCanQueensideCastle(previousMove->getPreviousBlackQueenSide());
    Box enPassant = previousMove->getPreviousEnPassant();
    currentBoardState->setEnPassantX(enPassant.x);
    currentBoardState->setEnPassantY(enPassant.y);


    if (currentTurn == 'w')
    {
        whiteThreatened =
        {
            previousMove->getPreviousWhiteThreatenedInfo(),
            previousMove->getPreviousWhiteAttackedInfo(),
            previousMove->getPreviousWhiteStraightLeftBox(),
            previousMove->getPreviousWhiteUpLeftBox(),
            previousMove->getPreviousWhiteStraightUpBox(),
            previousMove->getPreviousWhiteUpRightBox(),

            previousMove->getPreviousWhiteStraightRightBox(),
            previousMove->getPreviousWhiteDownRightBox(),
            previousMove->getPreviousWhiteStraightDownBox(),
            previousMove->getPreviousWhiteDownLeftBox(),
            previousMove->getPreviousAmountAttacked(),
            previousMove->getPreviousAttackedFromBox(),
            previousMove->getPreviousAttackedByKnight()

        };
        blackThreatened =
        {
            previousMove->getPreviousBlackThreatenedInfo(),
            previousMove->getPreviousBlackAttackedInfo(),
            previousMove->getPreviousBlackStraightLeftBox(),
            previousMove->getPreviousBlackUpLeftBox(),
            previousMove->getPreviousBlackStraightUpBox(),
            previousMove->getPreviousBlackUpRightBox(),

            previousMove->getPreviousBlackStraightRightBox(),
            previousMove->getPreviousBlackDownRightBox(),
            previousMove->getPreviousBlackStraightDownBox(),
            previousMove->getPreviousBlackDownLeftBox(),
            0,{-1,-1},false

        };
    }
    else
    {
        whiteThreatened =
        {
            previousMove->getPreviousWhiteThreatenedInfo(),
            previousMove->getPreviousWhiteAttackedInfo(),
            previousMove->getPreviousWhiteStraightLeftBox(),
            previousMove->getPreviousWhiteUpLeftBox(),
            previousMove->getPreviousWhiteStraightUpBox(),
            previousMove->getPreviousWhiteUpRightBox(),

            previousMove->getPreviousWhiteStraightRightBox(),
            previousMove->getPreviousWhiteDownRightBox(),
            previousMove->getPreviousWhiteStraightDownBox(),
            previousMove->getPreviousWhiteDownLeftBox(),
            0,{-1,-1},false

        };
        blackThreatened =
        {
            previousMove->getPreviousBlackThreatenedInfo(),
            previousMove->getPreviousBlackAttackedInfo(),
            previousMove->getPreviousBlackStraightLeftBox(),
            previousMove->getPreviousBlackUpLeftBox(),
            previousMove->getPreviousBlackStraightUpBox(),
            previousMove->getPreviousBlackUpRightBox(),

            previousMove->getPreviousBlackStraightRightBox(),
            previousMove->getPreviousBlackDownRightBox(),
            previousMove->getPreviousBlackStraightDownBox(),
            previousMove->getPreviousBlackDownLeftBox(),
            previousMove->getPreviousAmountAttacked(),
            previousMove->getPreviousAttackedFromBox(),
            previousMove->getPreviousAttackedByKnight()

        };
    }


    //now we deal with castling and en passant stuff.
    if (move.kingSideCastle)
    {
        board[boardXBoxes - 1][move.fromBox.y] = board[move.fromBox.x + 1][move.fromBox.y];
        board[move.fromBox.x + 1][move.fromBox.y] = 0;
        if (currentTurn == 'w')
        {
            whiteLocations.updateRook({ move.fromBox.x + 1,move.fromBox.y }, { boardXBoxes - 1,move.fromBox.y });
        }
        else
        {
            blackLocations.updateRook({ move.fromBox.x + 1,move.fromBox.y }, { boardXBoxes - 1,move.fromBox.y });
        }
    }
    else if (move.queenSideCastle)
    {
        board[0][move.fromBox.y] = board[move.toBox.x + 1][move.fromBox.y];
        board[move.toBox.x + 1][move.fromBox.y] = 0;
        if (currentTurn == 'w')
        {
            whiteLocations.updateRook({ move.toBox.x + 1,move.fromBox.y }, { 0,move.fromBox.y });
        }
        else
        {
            blackLocations.updateRook({ move.toBox.x + 1,move.fromBox.y }, { 0,move.fromBox.y });
        }
    }
    else if (move.enPassant)
    {
        if (currentTurn == 'w')
        {
            board[move.toBox.x][move.fromBox.y] = Piece::black | Piece::pawn;
            blackLocations.getPawnLocations().push_back({ move.toBox.x,move.fromBox.y });
        }
        else
        {
            board[move.toBox.x][move.fromBox.y] = Piece::white | Piece::pawn;
            whiteLocations.getPawnLocations().push_back({ move.toBox.x,move.fromBox.y });
        }
    }

    if (currentBoardState->getCurrentTurn() == 'b')
    {
        currentBoardState->setFullMoveClock(currentBoardState->getFullMoveClock() - 2);
    }

    currentBoardState->setHalfMoveClock(previousMove->getPreviousHalfMoveClock() - 1);

    delete previousMove;

    if (!moveStack.getSize()) reset();
    else nextTurn(currentBoardState);
}


void Board::attemptAddMove(Move move, BoardState* currentBoardState, std::vector<Move>& moves)
{
    char currentTurn = currentBoardState->getCurrentTurn();
    if (currentTurn == 'w')
    {
        if (whiteThreatened.amountAttacked >= 1)
        {
            if (doesBoxBlockAttack(move.toBox, currentBoardState))
            {
                moves.push_back({ {move.fromBox.x,move.fromBox.y},{move.toBox.x,move.toBox.y},move.kingSideCastle,move.queenSideCastle,move.enPassant,move.isPromotion,move.promotionType });
            }
        }
        else
        {
            moves.push_back({ {move.fromBox.x,move.fromBox.y},{move.toBox.x,move.toBox.y},move.kingSideCastle,move.queenSideCastle,move.enPassant,move.isPromotion,move.promotionType });
        }
    }
    else
    {
        if (blackThreatened.amountAttacked >= 1)
        {
            if (doesBoxBlockAttack(move.toBox, currentBoardState))
            {
                moves.push_back({ {move.fromBox.x,move.fromBox.y},{move.toBox.x,move.toBox.y},move.kingSideCastle,move.queenSideCastle,move.enPassant,move.isPromotion,move.promotionType });
            }
        }
        else
        {
            moves.push_back({ {move.fromBox.x,move.fromBox.y},{move.toBox.x,move.toBox.y},move.kingSideCastle,move.queenSideCastle,move.enPassant,move.isPromotion,move.promotionType });
        }
    }
}

void Board::addStraightUpMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;
    //going up on the board

    for (int currY = y - 1; currY >= 0; --currY)
    {
        if (currentBoardState->getBoard()[x][currY] == 0)
        {
            attemptAddMove({ {x,y},{x,currY}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(x, currY, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(x, currY, currentBoardState))
        {
            attemptAddMove({ {x,y},{x,currY}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addStraightDownMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;
    for (int currY = y + 1; currY < boardYBoxes; ++currY)
    {
        if (currentBoardState->getBoard()[x][currY] == 0)
        {
            attemptAddMove({ {x,y},{x,currY}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(x, currY, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(x, currY, currentBoardState))
        {
            attemptAddMove({ {x,y},{x,currY}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addStraightLeftMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    //going to the left on the board.
    int x = box.x;
    int y = box.y;
    for (int currX = x - 1; currX >= 0; --currX)
    {
        if (currentBoardState->getBoard()[currX][y] == 0)
        {
            attemptAddMove({ {x,y},{currX,y}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(currX, y, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(currX, y, currentBoardState))
        {
            attemptAddMove({ {x,y},{currX,y}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addStraightRightMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;
    //going to the right on the board.
    for (int currX = x + 1; currX < boardXBoxes; ++currX)
    {
        if (currentBoardState->getBoard()[currX][y] == 0)
        {
            attemptAddMove({ {x,y},{currX,y}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(currX, y, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(currX, y, currentBoardState))
        {
            attemptAddMove({ {x,y},{currX,y}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addUpRightMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    //going up and to the right
    int x = box.x;
    int y = box.y;
    for (int change = 1; x + change < boardXBoxes && y - change >= 0; ++change)
    {
        if (currentBoardState->getBoard()[x + change][y - change] == 0)
        {
            attemptAddMove({ {x,y},{x + change,y - change}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(x + change, y - change, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(x + change, y - change, currentBoardState))
        {
            attemptAddMove({ {x,y},{x + change,y - change}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addUpLeftMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    //going up and to the left.
    int x = box.x;
    int y = box.y;
    for (int change = 1; x - change >= 0 && y - change >= 0; ++change)
    {
        if (currentBoardState->getBoard()[x - change][y - change] == 0)
        {
            attemptAddMove({ {x,y},{x - change,y - change}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(x - change, y - change, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(x - change, y - change, currentBoardState))
        {
            attemptAddMove({ {x,y},{x - change,y - change}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addDownRightMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;
    for (int change = 1; x + change < boardXBoxes && y + change < boardYBoxes; ++change)
    {
        if (currentBoardState->getBoard()[x + change][y + change] == 0)
        {
            attemptAddMove({ {x,y},{x + change,y + change}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(x + change, y + change, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(x + change, y + change, currentBoardState))
        {
            attemptAddMove({ {x,y},{x + change,y + change}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}

void Board::addDownLeftMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    //going down and to the left
    int x = box.x;
    int y = box.y;
    for (int change = 1; x - change >= 0 && y + change < boardYBoxes; ++change)
    {
        if (currentBoardState->getBoard()[x - change][y + change] == 0)
        {
            attemptAddMove({ {x,y},{x - change,y + change}, false, false, false }, currentBoardState, moves);
        }
        else if (pieceIsCurrentPlayersPiece(x - change, y + change, currentBoardState))
        {
            break;
        }
        else if (!pieceIsCurrentPlayersPiece(x - change, y + change, currentBoardState))
        {
            attemptAddMove({ {x,y},{x - change,y + change}, false, false, false }, currentBoardState, moves);
            break;
        }
    }
}


void Board::addStraightUpPawnMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    //moving forward one.
    int x = box.x;
    int y = box.y;
    if (y - 1 >= 0)
    {
        //pawns cant take vertically.
        if (currentBoardState->getBoard()[x][y - 1] == 0)
        {
            //if it's a promotion
            if (y - 1 == 0)
            {
                attemptAddMove({ {x,y},{x,y - 1},false,false,false,true,'q' }, currentBoardState, moves);
                attemptAddMove({ {x,y},{x,y - 1},false,false,false,true,'r' }, currentBoardState, moves);
                attemptAddMove({ {x,y},{x,y - 1},false,false,false,true,'n' }, currentBoardState, moves);
                attemptAddMove({ {x,y},{x,y - 1},false,false,false,true,'b' }, currentBoardState, moves);
            }
            else
            {
                attemptAddMove({ {x,y},{x,y - 1},false,false,false,false,' ' }, currentBoardState, moves);
            }

            //we only can move forward 2 if the space is open as well.
            if (y == boardYBoxes - 2)  	//if it's in the starting position.
            {
                if (y - 2 >= 0)  	//this shouldn't be necessary except for tiny boards
                {
                    if (currentBoardState->getBoard()[x][y - 2] == 0)
                    {
                        attemptAddMove({ {x,y},{x,y - 2}, false, false, false,false,' ' }, currentBoardState, moves);
                    }
                }
            }
        }
    }
}

void Board::addStraightDownPawnMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;

    //moving forward one.
    if (y + 1 < boardYBoxes)
    {
        //pawns cant take vertically.
        if (currentBoardState->getBoard()[x][y + 1] == 0)
        {
            if (y + 1 == boardYBoxes - 1)
            {
                attemptAddMove({ {x,y},{x,y + 1},false,false,false,true,'r' }, currentBoardState, moves);
                attemptAddMove({ {x,y},{x,y + 1 },false,false,false,true,'n' }, currentBoardState, moves);
                attemptAddMove({ {x,y},{x,y + 1},false,false,false,true,'b' }, currentBoardState, moves);
                attemptAddMove({ {x,y},{x,y + 1},false,false,false,true,'q' }, currentBoardState, moves);
            }
            else
            {
                attemptAddMove({ {x,y},{x,y + 1},false,false,false,false,' ' }, currentBoardState, moves);
            }

            //we only can move forward 2 if the space is open as well.
            if (y == 1)  	//if it's in the starting position.
            {
                if (y + 2 < boardYBoxes)  	//this shouldn't be necessary except for tiny boards
                {
                    if (currentBoardState->getBoard()[x][y + 2] == 0)
                    {
                        attemptAddMove({ {x,y},{x,y + 2}, false, false, false,false, ' ' }, currentBoardState, moves);
                    }
                }
            }
        }
    }
}

void Board::addDownLeftPawnMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;
    //pawns capture diagonally.
    if (y + 1 < boardYBoxes)
    {
        //if we're not at the edge of the board
        if (x - 1 >= 0)
        {
            if (currentBoardState->getBoard()[x - 1][y + 1] != 0 && !pieceIsCurrentPlayersPiece(x - 1, y + 1, currentBoardState))
            {
                //promotion time
                if (y + 1 == boardYBoxes - 1)
                {
                    attemptAddMove({ {x,y},{x - 1,y + 1},false,false,false,true,'q' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x - 1,y + 1},false,false,false,true,'r' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x - 1,y + 1},false,false,false,true,'b' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x - 1,y + 1},false,false,false,true,'n' }, currentBoardState, moves);
                }
                else
                {
                    attemptAddMove({ {x,y},{x - 1,y + 1},false,false,false,false,' ' }, currentBoardState, moves);
                }
            }
            else if (x - 1 == currentBoardState->getEnPassantX() && y + 1 == currentBoardState->getEnPassantY())
            {
                if (canEnPassant({ x,y }, { x - 1,y + 1 }, currentBoardState))
                {
                    attemptAddMove({ {x,y},{x - 1,y + 1},false,false,true,false,' ' }, currentBoardState, moves);
                }
            }
        }
    }
}

void Board::addDownRightPawnMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;
    if (y + 1 < boardYBoxes)
    {
        if (x + 1 < boardXBoxes)
        {
            if (currentBoardState->getBoard()[x + 1][y + 1] != 0 && !pieceIsCurrentPlayersPiece(x + 1, y + 1, currentBoardState))
            {
                if (y + 1 == boardYBoxes - 1)
                {
                    attemptAddMove({ {x,y},{x + 1,y + 1},false,false,false,true,'r' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x + 1,y + 1},false,false,false,true,'q' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x + 1,y + 1},false,false,false,true,'b' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x + 1,y + 1 }, false, false, false, true, 'n'}, currentBoardState, moves);
                }
                else
                {
                    attemptAddMove({ {x,y},{x + 1,y + 1},false,false,false,false,' ' }, currentBoardState, moves);
                }
            }
            else if (x + 1 == currentBoardState->getEnPassantX() && y + 1 == currentBoardState->getEnPassantY())
            {
                if (canEnPassant({ x,y }, { x + 1,y + 1 }, currentBoardState))
                {
                    attemptAddMove({ {x,y},{x + 1,y + 1},false,false,true,false,' ' }, currentBoardState, moves);
                }
            }
        }
    }
}

void Board::addUpRightPawnMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;

    //pawns capture diagonally.
    if (y - 1 >= 0)
    {
        if (x + 1 < boardXBoxes)
        {
            if (currentBoardState->getBoard()[x + 1][y - 1] != 0 && !pieceIsCurrentPlayersPiece(x + 1, y - 1, currentBoardState))
            {
                if (y - 1 == 0)
                {
                    attemptAddMove({ {x,y},{x + 1,y - 1}, false, false, false,true,'q' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x + 1,y - 1}, false, false, false,true,'r' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x + 1,y - 1}, false, false, false,true,'n' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x + 1,y - 1}, false, false, false,true,'b' }, currentBoardState, moves);
                }
                else
                {
                    attemptAddMove({ {x,y},{x + 1,y - 1}, false, false, false,false,' ' }, currentBoardState, moves);
                }
            }
            //en passant
            else if (x + 1 == currentBoardState->getEnPassantX() && y - 1 == currentBoardState->getEnPassantY())
            {
                //we can only en passant if it doesn't put our king in check.
                if (canEnPassant({ x,y }, { x + 1,y - 1 }, currentBoardState))
                {
                    attemptAddMove({ {x,y},{x + 1,y - 1},false,false,true,false,' ' }, currentBoardState, moves);
                }
            }
        }
    }
}

void Board::addUpLeftPawnMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    //if we're not at the edge of the board
    int x = box.x;
    int y = box.y;
    if (y - 1 >= 0)
    {
        if (x - 1 >= 0)
        {
            if (currentBoardState->getBoard()[x - 1][y - 1] != 0 && !pieceIsCurrentPlayersPiece(x - 1, y - 1, currentBoardState))
            {
                if (y - 1 == 0)
                {
                    attemptAddMove({ {x,y},{x - 1,y - 1}, false, false, false,true,'q' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x - 1,y - 1}, false, false, false,true,'r' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x - 1,y - 1}, false, false, false,true,'n' }, currentBoardState, moves);
                    attemptAddMove({ {x,y},{x - 1,y - 1}, false, false, false,true,'b' }, currentBoardState, moves);
                }
                else
                {
                    attemptAddMove({ {x,y},{x - 1,y - 1}, false, false, false,false,' ' }, currentBoardState, moves);
                }

            }
            //en passant
            else if (x - 1 == currentBoardState->getEnPassantX() && y - 1 == currentBoardState->getEnPassantY())
            {
                if (canEnPassant({ x,y }, { x - 1,y - 1 }, currentBoardState))
                {
                    attemptAddMove({ {x,y},{x - 1,y - 1},false,false,true,false,' ' }, currentBoardState, moves);
                }
            }
        }
    }
}

//update castling status.
void Board::updateCastling(Box fromBox, Box toBox, BoardState* currentBoardState)
{
    //if the king moved.
    int queenSideX = 0;
    int kingSideX = boardXBoxes - 1;
    int whiteY = boardYBoxes - 1;
    int blackY = 0;

    uint8_t** board = boardState->getBoard();
    if (currentBoardState->getCurrentTurn() == 'w')
    {
        if ((board[fromBox.x][fromBox.y] & Piece::king) == Piece::king)
        {
            currentBoardState->setWhiteCanKingsideCastle(false);
            currentBoardState->setWhiteCanQueensideCastle(false);
            currentBoardState->setWhiteCanQueensideCastle(false);
        }
        if (fromBox.x == queenSideX && fromBox.y == whiteY)
        {
            currentBoardState->setWhiteCanQueensideCastle(false);
        }
        else if (fromBox.x == kingSideX && fromBox.y == whiteY)
        {
            currentBoardState->setWhiteCanKingsideCastle(false);
        }
        if (toBox.x == kingSideX && toBox.y == blackY)
        {
            currentBoardState->setBlackCanKingsideCastle(false);
        }
        else if (toBox.x == queenSideX && toBox.y == blackY)
        {
            currentBoardState->setBlackCanQueensideCastle(false);
        }
    }
    else
    {
        if ((board[fromBox.x][fromBox.y] & Piece::king) == Piece::king)
        {
            currentBoardState->setBlackCanKingsideCastle(false);
            currentBoardState->setBlackCanQueensideCastle(false);
        }
        if (fromBox.x == queenSideX && fromBox.y == blackY)
        {
            currentBoardState->setBlackCanQueensideCastle(false);
        }
        else if (fromBox.x == kingSideX && fromBox.y == blackY)
        {
            currentBoardState->setBlackCanKingsideCastle(false);
        }
        //if the move takes a rook.
        if (toBox.x == kingSideX && toBox.y == whiteY)
        {
            currentBoardState->setWhiteCanKingsideCastle(false);
        }
        else if (toBox.x == queenSideX && toBox.y == whiteY)
        {
            currentBoardState->setWhiteCanQueensideCastle(false);
        }
    }
}

void Board::updateEnPassant(Box fromBox, Box toBox, BoardState* currentBoardState)
{
    if ((currentBoardState->getBoard()[fromBox.x][fromBox.y] & Piece::pawn) == Piece::pawn)
    {
        if (abs(fromBox.y - toBox.y) == 2)
        {
            currentBoardState->setEnPassantX(fromBox.x);
            currentBoardState->setEnPassantY((fromBox.y + toBox.y) / 2);
        }
        else
        {
            currentBoardState->setEnPassantX(-1);
            currentBoardState->setEnPassantY(-1);
        }
    }
    else
    {
        currentBoardState->setEnPassantX(-1);
        currentBoardState->setEnPassantY(-1);
        currentBoardState->setEnPassantY(-1);
    }
}

void Board::updateHighlightKingBox()
{
    if (gameState == DRAW)
    {
        findKingLocation(&drawFirstKing, boardState);
        switchTurns(boardState);
        findKingLocation(&drawSecondKing, boardState);
        return;
    }
    if (kingInCheck(boardState))
    {
        findKingLocation(&highlightKingBox, boardState);
    }
    else
    {
        highlightKingBox = {-1,-1};
    }
    if (gameState == LOST)
    {
        switchTurns(boardState);
        findKingLocation(&winnerKing, boardState);
    }
}

void Board::createHighlightMoves(Box box)
{
    highlightBoxes.clear();
    for (unsigned i = 0; i < legalMoves.size(); i++)
    {
        if (legalMoves[i].fromBox == box)
        {
            highlightBoxes.push_back( legalMoves[i].toBox );
        }
    }
}

void Board::makeRandomMove(BoardState* currentBoardState)
{
    if (gameState != RUNNING) return;
    //legalMoves = calculateLegalMoves(currentBoardState);
    int choice = rand() % legalMoves.size();
    makeMove(legalMoves.at(choice), currentBoardState);

    nextTurn(currentBoardState);
}
