#pragma once
#include "SDL.h"
#include "Chess_Move.h"
#include "Chess_BoardState.h"
#include "Chess_MoveStack.h"
#include "Chess_Piece.h"
#include "Chess_PieceLocations.h"
#include "Chess_KingThreatenedInfo.h"
#include <vector>

enum GameState
{
    RUNNING = 0, LOST, DRAW
};
class Board
{
private:

    SDL_Color boardColor1;
    SDL_Color boardColor2;

    BoardState* boardState;

    PieceLocations whiteLocations;
    PieceLocations blackLocations;

    KingThreatenedInfo whiteThreatened;
    KingThreatenedInfo blackThreatened;


    //moving piece stuff.
    bool draggingPiece;
    Box draggingPieceBox;
    SDL_Texture* draggingPieceTexture;

    //Move stuff
    std::vector<Move> legalMoves;

    std::vector<Box> highlightBoxes;

    Box highlightKingBox;
    Box winnerKing;
    Box drawFirstKing;
    Box drawSecondKing;

    Move promotionMove;
    bool waitingForPromotionChoice;

    GameState gameState;
    char winner;

    MoveStack moveStack;

public:
    static int boxXWidth;
    static int boxYHeight;
    static int boardXBoxes;
    static int boardYBoxes;

    ///--------------Board_render.cpp--------------///

    void render(BoardState*);
    void renderBoard();
    void loadButton();
    void renderOutsideBoard();
    void renderDraggedPiece();

    void renderPieces(BoardState*);
    void renderPiece(Box,BoardState*);
    void renderPieceTexture(SDL_Texture*, int x, int y);

    SDL_Texture* getTextureAtLocation(int x, int y, BoardState*);

    void renderAttackedSquares();

    void renderBox(Box, SDL_Color);
    void renderKingBox();
    void renderHighlightMoves();
    void renderPreviousMove();

    void renderPromotionOptions();

    ///-----------------Board.cpp------------------///

    void init();
    void reset();
    ~Board();
    void update();

    void initializePieceLocations(BoardState*);
    void loadBoardFromFen(const char* fen, BoardState*);

    void handleMouseButtonDown(BoardState*);
    void handleMouseButtonUp(BoardState*);
    void attemptPickupPiece(Box, BoardState*);
    void attemptPlacePiece(Box, BoardState*);
    void stopDraggingPiece();

    void tryPickingPromotionPiece(int, BoardState*);
    void promotePiece(BoardState*, char);

    void nextTurn(BoardState* boardState);
    void switchTurns(BoardState*);

    void clearMoves();

    void makeMove(Move, BoardState*);
    void unMakeMove(BoardState*);

    void attemptAddMove(Move move, BoardState* currentBoardState, std::vector<Move>& moves);

    void addStraightUpMoves(Box, BoardState*, std::vector<Move>&);
    void addStraightDownMoves(Box, BoardState*, std::vector<Move>&);
    void addStraightLeftMoves(Box, BoardState*, std::vector<Move>&);
    void addStraightRightMoves(Box, BoardState*, std::vector<Move>&);

    void addUpRightMoves(Box, BoardState*, std::vector<Move>&);
    void addDownRightMoves(Box, BoardState*, std::vector<Move>&);
    void addUpLeftMoves(Box, BoardState*, std::vector<Move>&);
    void addDownLeftMoves(Box, BoardState*, std::vector<Move>&);

    void addStraightUpPawnMoves(Box, BoardState*, std::vector<Move>&);
    void addStraightDownPawnMoves(Box, BoardState*, std::vector<Move>&);
    void addDownLeftPawnMoves(Box, BoardState*, std::vector<Move>&);
    void addDownRightPawnMoves(Box, BoardState*, std::vector<Move>&);
    void addUpRightPawnMoves(Box, BoardState*, std::vector<Move>&);
    void addUpLeftPawnMoves(Box, BoardState*, std::vector<Move>&);

    void updateCastling(Box fromBox, Box toBox, BoardState*);
    void updateEnPassant(Box fromBox, Box toBox, BoardState*);
    void updateHighlightKingBox();
    void createHighlightMoves(Box );

    void makeRandomMove(BoardState*);


    ///--------------Board_caculateMove.cpp-----------------///

    int totalPossibleFutureBoardPositions(BoardState*, int depth);
    void calculateBoardStates();

    std::vector<Move> calculateLegalMoves(BoardState*);

    void calculateRookPseudoLegalMoves(Box, BoardState*, std::vector<Move>&);
    void calculateBishopPseudoLegalMoves(Box, BoardState*, std::vector<Move>&);
    void calculateKnightPseudoLegalMoves(Box, BoardState*, std::vector<Move>&);
    void calculateQueenPseudoLegalMoves(Box, BoardState*, std::vector<Move>&);
    void calculatePawnUpPseudoLegalMoves(Box, BoardState*, std::vector<Move>&);
    void calculatePawnDownPseudoLegalMoves(Box, BoardState*, std::vector<Move>&);

    void calculateRookLegalMoves(Box, Box, BoardState*, std::vector<Move>&,KingThreatenedInfo);
    void calculateKingLegalMoves(Box, BoardState*, std::vector<Move>&, KingThreatenedInfo);
    void calculateBishopLegalMoves(Box, Box, BoardState*, std::vector<Move>&,KingThreatenedInfo);
    void calculateQueenLegalMoves(Box, Box, BoardState*, std::vector<Move>&, KingThreatenedInfo);
    void calculatePawnLegalMoves(Box, Box, BoardState*, std::vector<Move>&, KingThreatenedInfo);
    void calculateKnightLegalMoves(Box, Box, BoardState*, std::vector<Move>&, KingThreatenedInfo);
    void calculateCastlingLegalMoves(Box, BoardState*, std::vector<Move>&);


    ///--------------------Board_threats.cpp-----------------------///

    void clearThreats();
    void initializeKingsThreatened(BoardState*);

    void updateAllThreats(char curentTurn, BoardState* currentBoardState);
    void updateMoveToThreats(Move, BoardState*);
    void updateThreats(Move, BoardState*);

    void updateStraightUpThreats(char currentTurn, BoardState*);
    void updateStraightLeftThreats(char currentTurn, BoardState*);
    void updateStraightDownThreats(char currentTurn, BoardState*);
    void updateStraightRightThreats(char currentTurn, BoardState*);

    void updateUpLeftThreats(char currentTurn, BoardState*);
    void updateUpRightThreats(char currentTurn, BoardState*);
    void updateDownLeftThreats(char currentTurn, BoardState*);
    void updateDownRightThreats(char currentTurn, BoardState*);


    ///--------------Board_query.cpp--------------///

    int getWidth();
    int getHeight();
    BoardState* getBoardState();
    void printBoardState(BoardState*);

    bool pieceIsCurrentPlayersPiece(int x, int y, BoardState*);

    bool inLegalMoves(struct Move&);

    bool isEnPassant(Box fromBox, Box toBox, BoardState*);
    bool canEnPassant(Box, Box, BoardState*);

    void findKingLocation(Box*, BoardState*);

    bool kingAttacked(BoardState* currentBoardState);
    bool kingInCheck(BoardState* currentBoardState);
    bool squareAttacked(Box box, BoardState* currentBoardState);
    bool doesBoxBlockAttack(Box, BoardState* currentBoardState);

    int isGameOver(BoardState* currentBoardState);

    bool inSameRow(Box, Box);
    bool inSameCol(Box, Box);

    //bottom left to top right
    bool inSameDiagonal(Box, Box);
    //top left to bottom right
    bool inSameReverseDiagonal(Box, Box);
};

