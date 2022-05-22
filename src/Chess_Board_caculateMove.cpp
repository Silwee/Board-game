#include "Chess_Board.h"
#include <iostream>
#define totalTests 5

/** The 2 function below caculating all possible positions of the current chess board after n moves (perft result)*/
void Board::calculateBoardStates()
{
    for (int i = 1; i <= totalTests; i++)
    {
        int initialTime = SDL_GetTicks();
        std::cout << "Total Board states in "<< i <<" moves: " << totalPossibleFutureBoardPositions(boardState, i) <<std::endl;
        std::cout << "Took : " << SDL_GetTicks() - initialTime << " Milliseconds" << std::endl;
        initialTime = SDL_GetTicks();
    }
}
int Board::totalPossibleFutureBoardPositions(BoardState* currentBoardState, int depth)
{
    int totalAmount = 0;
    if (depth == 0)
    {
        return 1;		//if we're at the end then this is a board state
    }
    std::vector<Move> legalMoves = calculateLegalMoves(currentBoardState);
    for (unsigned i = 0; i < legalMoves.size(); i++)
    {
        makeMove(legalMoves[i], currentBoardState);

        int amountOfMoves = totalPossibleFutureBoardPositions(currentBoardState, depth - 1);
        if (depth == totalTests)
        {
            std::cout <<  char('a' + legalMoves[i].fromBox.x) << 8 - legalMoves[i].fromBox.y  << char('a' + legalMoves[i].toBox.x) << 8 - legalMoves[i].toBox.y << ": " << amountOfMoves << std::endl;
        }
        totalAmount += amountOfMoves;
        unMakeMove(currentBoardState);
    }
    return totalAmount;
}

/** Pseudo legal move: the possible move one's piece can make if we don't care about the king that can be attacked
*   Legal move: the pseudo legal move that isn't made the king in check
*   The moves that is pseudo legal but is not legal:
**      - Move the king in a position that was attacked
**      - Move the piece (and pawn) away from the attacked line that attacked the king (absolute pinned)
**      - Castling when the king or the cells in the castling way is in attacked
*/

std::vector<Move> Board::calculateLegalMoves(BoardState* currentBoardState)
{
    std::vector<Move> currentLegal;
    char currentTurn = currentBoardState->getCurrentTurn();
    Box kingLocation;

    if (currentTurn == 'w')
    {
        kingLocation = whiteLocations.getKingLocation();
        calculateKingLegalMoves(kingLocation, currentBoardState, currentLegal, whiteThreatened);
        //we can only move the king if it's in double check
        if (whiteThreatened.amountAttacked != 2)
        {
            for (unsigned i = 0; i < whiteLocations.getRookLocations().size(); i++)
            {
                calculateRookLegalMoves(whiteLocations.getRookLocations()[i], kingLocation, currentBoardState, currentLegal, whiteThreatened);
            }
            for (unsigned i = 0; i < whiteLocations.getKnightLocations().size(); i++)
            {
                calculateKnightLegalMoves(whiteLocations.getKnightLocations()[i], kingLocation, currentBoardState, currentLegal, whiteThreatened);
            }
            for (unsigned i = 0; i < whiteLocations.getQueenLocations().size(); i++)
            {
                calculateQueenLegalMoves(whiteLocations.getQueenLocations()[i], kingLocation, currentBoardState, currentLegal, whiteThreatened);
            }
            for (unsigned i = 0; i < whiteLocations.getPawnLocations().size(); i++)
            {
                calculatePawnLegalMoves(whiteLocations.getPawnLocations()[i], kingLocation, currentBoardState, currentLegal, whiteThreatened);
            }
            for (unsigned i = 0; i < whiteLocations.getBishopLocations().size(); i++)
            {
                calculateBishopLegalMoves(whiteLocations.getBishopLocations()[i], kingLocation, currentBoardState, currentLegal, whiteThreatened);
            }
        }
    }
    else
    {
        kingLocation = blackLocations.getKingLocation();
        calculateKingLegalMoves(kingLocation, currentBoardState, currentLegal, blackThreatened);
        //we can only move the king if it's in double check
        if (blackThreatened.amountAttacked != 2)
        {
            for (unsigned i = 0; i < blackLocations.getRookLocations().size(); i++)
            {
                calculateRookLegalMoves(blackLocations.getRookLocations()[i], kingLocation, currentBoardState, currentLegal, blackThreatened);
            }
            for (unsigned i = 0; i < blackLocations.getKnightLocations().size(); i++)
            {
                calculateKnightLegalMoves(blackLocations.getKnightLocations()[i], kingLocation, currentBoardState, currentLegal, blackThreatened);
            }
            for (unsigned i = 0; i < blackLocations.getQueenLocations().size(); i++)
            {
                calculateQueenLegalMoves(blackLocations.getQueenLocations()[i], kingLocation, currentBoardState, currentLegal, blackThreatened);
            }
            for (unsigned i = 0; i < blackLocations.getPawnLocations().size(); i++)
            {
                calculatePawnLegalMoves(blackLocations.getPawnLocations()[i], kingLocation, currentBoardState, currentLegal, blackThreatened);
            }
            for (unsigned i = 0; i < blackLocations.getBishopLocations().size(); i++)
            {
                calculateBishopLegalMoves(blackLocations.getBishopLocations()[i], kingLocation, currentBoardState, currentLegal, blackThreatened);
            }
        }

    }
    return currentLegal;
}

void Board::calculateRookPseudoLegalMoves(Box box, BoardState* currentBoardState, std::vector<Move>& currentPseudo)
{
    addStraightRightMoves(box, currentBoardState, currentPseudo);
    addStraightLeftMoves(box, currentBoardState, currentPseudo);
    addStraightUpMoves(box, currentBoardState, currentPseudo);
    addStraightDownMoves(box, currentBoardState, currentPseudo);
}

void Board::calculateBishopPseudoLegalMoves(Box box, BoardState* currentBoardState, std::vector<Move>& currentPseudo)
{
    addDownRightMoves(box, currentBoardState, currentPseudo);
    addDownLeftMoves(box, currentBoardState, currentPseudo);
    addUpLeftMoves(box, currentBoardState, currentPseudo);
    addUpRightMoves(box, currentBoardState, currentPseudo);
}

void Board::calculateQueenPseudoLegalMoves(Box box, BoardState* currentBoardState, std::vector<Move>& currentPseudo)
{
    addDownRightMoves(box, currentBoardState, currentPseudo);
    addDownLeftMoves(box, currentBoardState, currentPseudo);
    addUpLeftMoves(box, currentBoardState, currentPseudo);
    addUpRightMoves(box, currentBoardState, currentPseudo);

    addStraightRightMoves(box, currentBoardState, currentPseudo);
    addStraightLeftMoves(box, currentBoardState, currentPseudo);
    addStraightUpMoves(box, currentBoardState, currentPseudo);
    addStraightDownMoves(box, currentBoardState, currentPseudo);
}

void Board::calculateKnightPseudoLegalMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    int x = box.x;
    int y = box.y;

    //knight moving up.
    uint8_t** board = currentBoardState->getBoard();
    if (y - 2 >= 0)
    {
        if (x + 1 < boardXBoxes)
        {
            if (board[x + 1][y - 2] == 0 || !pieceIsCurrentPlayersPiece(x + 1, y - 2, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x + 1, y - 2}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
        if (x - 1 >= 0)
        {
            if (board[x - 1][y - 2] == 0 || !pieceIsCurrentPlayersPiece(x - 1, y - 2, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x - 1, y - 2}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
    }
    //knight moving down
    if (y + 2 < boardYBoxes)
    {
        if (x + 1 < boardXBoxes)
        {
            if (board[x + 1][y + 2] == 0 || !pieceIsCurrentPlayersPiece(x + 1, y + 2, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x + 1, y + 2}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
        if (x - 1 >= 0)
        {
            if (board[x - 1][y + 2] == 0 || !pieceIsCurrentPlayersPiece(x - 1, y + 2, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x - 1, y + 2}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
    }
    //knight moving left
    if (x - 2 >= 0)
    {
        if (y + 1 < boardYBoxes)
        {
            if (board[x - 2][y + 1] == 0 || !pieceIsCurrentPlayersPiece(x - 2, y + 1, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x - 2, y + 1}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
        if (y - 1 >= 0)
        {
            if (board[x - 2][y - 1] == 0 || !pieceIsCurrentPlayersPiece(x - 2, y - 1, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x - 2, y - 1}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
    }
    //knight moving right
    if (x + 2 < boardXBoxes)
    {
        if (y + 1 < boardYBoxes)
        {
            if (board[x + 2][y + 1] == 0 || !pieceIsCurrentPlayersPiece(x + 2, y + 1, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x + 2, y + 1}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
        if (y - 1 >= 0)
        {
            if (board[x + 2][y - 1] == 0 || !pieceIsCurrentPlayersPiece(x + 2, y - 1, currentBoardState))
            {
                attemptAddMove({ {x, y}, {x + 2, y - 1}, false, false, false,false,' ' }, currentBoardState, moves);
            }
        }
    }
}

void Board::calculatePawnDownPseudoLegalMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    addStraightDownPawnMoves(box, currentBoardState, moves);
    addDownLeftPawnMoves(box, currentBoardState, moves);
    addDownRightPawnMoves(box, currentBoardState, moves);
}
void Board::calculatePawnUpPseudoLegalMoves(Box box, BoardState* currentBoardState, std::vector<Move>& moves)
{
    addStraightUpPawnMoves(box, currentBoardState, moves);
    addUpLeftPawnMoves(box, currentBoardState, moves);
    addUpRightPawnMoves(box, currentBoardState, moves);
}



void Board::calculateRookLegalMoves(Box rookBox, Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves, KingThreatenedInfo currentKingInfo)
{
    //checking if the rook is absolute pinned (blocking other's piece attacked the king)
    bool addAll = true;

    //if the rook is pinned from above (or below) we can only move the rook straight up and down.
    if (inSameCol(rookBox, kingBox))
    {
        //if the rook is above the king.
        if (rookBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightUpThreatened) == KingThreatenedInfo::straightUpThreatened)
            {
                if (currentKingInfo.straightUpBox.y < rookBox.y)
                {
                    addStraightDownMoves(rookBox, currentBoardState, currentLegalMoves);
                    addStraightUpMoves(rookBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
        //the rook is below the king.
        else if (rookBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightDownThreatened) == KingThreatenedInfo::straightDownThreatened)
            {
                if (currentKingInfo.straightDownBox.y > rookBox.y)
                {
                    addStraightDownMoves(rookBox, currentBoardState, currentLegalMoves);
                    addStraightUpMoves(rookBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }

            }
        }
    }

    //if the rook is pinned from the left (or right) we can only move the rook straight left and right.
    else if (inSameRow(rookBox, kingBox))
    {
        //rook is to the right of the king.
        if (rookBox.x > kingBox.x)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightRightThreatened) == KingThreatenedInfo::straightRightThreatened)
            {
                if (currentKingInfo.straightRightBox.x > rookBox.x)
                {
                    addStraightRightMoves(rookBox, currentBoardState, currentLegalMoves);
                    addStraightLeftMoves(rookBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }

        }
        else if (rookBox.x < kingBox.x)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightLeftThreatened) == KingThreatenedInfo::straightLeftThreatened)
            {
                if (currentKingInfo.straightLeftBox.x < rookBox.x)
                {
                    addStraightRightMoves(rookBox, currentBoardState, currentLegalMoves);
                    addStraightLeftMoves(rookBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }

    }

    //if the rook is pinned diagonally, we can't move the rook at all.
    else if (inSameDiagonal(rookBox, kingBox))
    {
        //if the rook is above the king.
        if (rookBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upRightThreatened) == KingThreatenedInfo::upRightThreatened)
            {
                if (currentKingInfo.upRightBox.x > rookBox.x)
                {
                    addAll = false;
                }
            }
        }
        //if the rook is below the king
        if (rookBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downLeftThreatened) == KingThreatenedInfo::downLeftThreatened)
            {
                if (currentKingInfo.downLeftBox.x < rookBox.x)
                {
                    addAll = false;
                }
            }
        }
    }

    //if the rook is pinned diagonally, we can't move the rook at all.
    else if (inSameReverseDiagonal(rookBox, kingBox))
    {
        //if the rook is above the king.
        if (rookBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upLeftThreatened) == KingThreatenedInfo::upLeftThreatened)
            {
                if (currentKingInfo.upLeftBox.x < rookBox.x)
                {
                    addAll = false;
                }
            }
        }
        //if the rook is below the king
        if (rookBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downRightThreatened) == KingThreatenedInfo::downRightThreatened)
            {
                if (currentKingInfo.downRightBox.x > rookBox.x)
                {
                    addAll = false;
                }
            }
        }
    }

    //if the rook is not pinned, we can just add on all the possible rook moves.
    if (addAll)
    {
        calculateRookPseudoLegalMoves(rookBox, currentBoardState, currentLegalMoves);
    }

}

void Board::calculateBishopLegalMoves(Box bishopBox, Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves, KingThreatenedInfo currentKingInfo)
{
    //checking if the bishop is absolute pinned
    bool addAll = true;

    //if the bishop is pinned in a straight line we can't move the bishop
    if (inSameCol(bishopBox, kingBox))
    {
        //if the bishop is above the king.
        if (bishopBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightUpThreatened) == KingThreatenedInfo::straightUpThreatened)
            {
                if (currentKingInfo.straightUpBox.y < bishopBox.y)
                {
                    addAll = false;
                }
            }
        }
        //the bishop is below the king.
        else if (bishopBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightDownThreatened) == KingThreatenedInfo::straightDownThreatened)
            {
                if (currentKingInfo.straightDownBox.y > bishopBox.y)
                {
                    addAll = false;
                }

            }
        }
    }
    else if (inSameRow(bishopBox, kingBox))
    {
        //bishop is to the right
        if (bishopBox.x > kingBox.x)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightRightThreatened) == KingThreatenedInfo::straightRightThreatened)
            {
                if (currentKingInfo.straightRightBox.x > bishopBox.x)
                {
                    addAll = false;
                }
            }
        }
        else if (bishopBox.x < kingBox.x)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightLeftThreatened) == KingThreatenedInfo::straightLeftThreatened)
            {
                if (currentKingInfo.straightLeftBox.x < bishopBox.x)
                {
                    addAll = false;
                }
            }
        }
    }

    //if the bishop is pinned diagonally, we can only moves the bishop in that diagonal
    else if (inSameDiagonal(bishopBox, kingBox))
    {
        //if the bishop is above the king.
        if (bishopBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upRightThreatened) == KingThreatenedInfo::upRightThreatened)
            {
                if (currentKingInfo.upRightBox.x > bishopBox.x)
                {
                    addUpRightMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addDownLeftMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
        //if the bishop is below the king
        if (bishopBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downLeftThreatened) == KingThreatenedInfo::downLeftThreatened)
            {
                if (currentKingInfo.downLeftBox.x < bishopBox.x)
                {
                    addUpRightMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addDownLeftMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
    }
    else if (inSameReverseDiagonal(bishopBox, kingBox))
    {
        //if the bishop is above the king
        if (bishopBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upLeftThreatened) == KingThreatenedInfo::upLeftThreatened)
            {
                if (currentKingInfo.upLeftBox.x < bishopBox.x)
                {
                    addUpLeftMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addDownRightMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
        //if the bishop is below the king
        if (bishopBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downRightThreatened) == KingThreatenedInfo::downRightThreatened)
            {
                if (currentKingInfo.downRightBox.x > bishopBox.x)
                {
                    addUpLeftMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addDownRightMoves(bishopBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
    }

    //if the bishop is not pinned, we can just add on all the possible bishop moves.
    if (addAll)
    {
        calculateBishopPseudoLegalMoves(bishopBox, currentBoardState, currentLegalMoves);
    }
}

void Board::calculateQueenLegalMoves(Box queenBox, Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves, KingThreatenedInfo currentKingInfo)
{
    //checking is the queen is absolute pinned
    bool addAll = true;

    //if the queen is pinned, we can only move the queen in the pinned line

    if (inSameCol(queenBox, kingBox))
    {
        //if the queen is above the king.
        if (queenBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightUpThreatened) == KingThreatenedInfo::straightUpThreatened)
            {
                if (currentKingInfo.straightUpBox.y < queenBox.y)
                {
                    addStraightUpMoves(queenBox, currentBoardState, currentLegalMoves);
                    addStraightDownMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
        //the queen is below the king.
        else if (queenBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightDownThreatened) == KingThreatenedInfo::straightDownThreatened)
            {
                if (currentKingInfo.straightDownBox.y > queenBox.y)
                {
                    addStraightUpMoves(queenBox, currentBoardState, currentLegalMoves);
                    addStraightDownMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
    }
    else if (inSameRow(queenBox, kingBox))
    {
        //queen is to the right
        if (queenBox.x > kingBox.x)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightRightThreatened) == KingThreatenedInfo::straightRightThreatened)
            {
                if (currentKingInfo.straightRightBox.x > queenBox.x)
                {
                    addStraightLeftMoves(queenBox, currentBoardState, currentLegalMoves);
                    addStraightRightMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
        else if (queenBox.x < kingBox.x)
        {
            //add no queen moves
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightLeftThreatened) == KingThreatenedInfo::straightLeftThreatened)
            {
                if (currentKingInfo.straightLeftBox.x < queenBox.x)
                {
                    addStraightLeftMoves(queenBox, currentBoardState, currentLegalMoves);
                    addStraightRightMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
    }
    else if (inSameDiagonal(queenBox, kingBox))
    {
        //queen above king
        if (queenBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upRightThreatened) == KingThreatenedInfo::upRightThreatened)
            {
                if (currentKingInfo.upRightBox.x > queenBox.x)
                {
                    addUpRightMoves(queenBox, currentBoardState, currentLegalMoves);
                    addDownLeftMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }

            }
        }
        //king below king
        if (queenBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downLeftThreatened) == KingThreatenedInfo::downLeftThreatened)
            {
                if (currentKingInfo.downLeftBox.x < queenBox.x)
                {
                    addUpRightMoves(queenBox, currentBoardState, currentLegalMoves);
                    addDownLeftMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
    }
    else if (inSameReverseDiagonal(queenBox, kingBox))
    {
        //if the queen is above the king
        if (queenBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upLeftThreatened) == KingThreatenedInfo::upLeftThreatened)
            {
                if (currentKingInfo.upLeftBox.x < queenBox.x)
                {
                    addUpLeftMoves(queenBox, currentBoardState, currentLegalMoves);
                    addDownRightMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
        //if the queen is below the king
        if (queenBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downRightThreatened) == KingThreatenedInfo::downRightThreatened)
            {
                if (currentKingInfo.downRightBox.x > queenBox.x)
                {
                    addUpLeftMoves(queenBox, currentBoardState, currentLegalMoves);
                    addDownRightMoves(queenBox, currentBoardState, currentLegalMoves);
                    addAll = false;
                }
            }
        }
    }

    //if the queen isn't pinned, we can just add on all the possible queen moves.
    if (addAll)
    {
        calculateQueenPseudoLegalMoves(queenBox, currentBoardState, currentLegalMoves);
    }
}

void Board::calculateKnightLegalMoves(Box knightBox, Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves, KingThreatenedInfo currentKingInfo)
{
    //checking is the knight is absolute pinned
    bool addAll = true;

    //if the knight is pinned, we can't move it at all.

    if (inSameCol(knightBox, kingBox))
    {
        //if the knight is above the king.
        if (knightBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightUpThreatened) == KingThreatenedInfo::straightUpThreatened)
            {
                if (currentKingInfo.straightUpBox.y < knightBox.y)
                {
                    addAll = false;
                }
            }
        }
        //the knight is below the king.
        else if (knightBox.y > kingBox.y)
        {
            //no knight moves
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightDownThreatened) == KingThreatenedInfo::straightDownThreatened)
            {
                if (currentKingInfo.straightDownBox.y > knightBox.y)
                {
                    addAll = false;
                }
            }
        }
    }
    else if (inSameRow(knightBox, kingBox))
    {
        //knight is to the right
        if (knightBox.x > kingBox.x)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightRightThreatened) == KingThreatenedInfo::straightRightThreatened)
            {
                if (currentKingInfo.straightRightBox.x > knightBox.x)
                {
                    addAll = false;
                }
            }
        }
        else if (knightBox.x < kingBox.x)
        {
            //add no knight moves
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightLeftThreatened) == KingThreatenedInfo::straightLeftThreatened)
            {
                if (currentKingInfo.straightLeftBox.x < knightBox.x)
                {
                    addAll = false;
                }
            }
        }
    }
    else if (inSameDiagonal(knightBox, kingBox))
    {
        //knight above king
        if (knightBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upRightThreatened) == KingThreatenedInfo::upRightThreatened)
            {
                if (currentKingInfo.upRightBox.x > knightBox.x)
                {
                    addAll = false;
                }
            }
        }
        //knight below king
        if (knightBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downLeftThreatened) == KingThreatenedInfo::downLeftThreatened)
            {
                if (currentKingInfo.downLeftBox.x < knightBox.x)
                {
                    addAll = false;
                }
            }
        }
    }
    else if (inSameReverseDiagonal(knightBox, kingBox))
    {
        //if the knight is above the king
        if (knightBox.y < kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upLeftThreatened) == KingThreatenedInfo::upLeftThreatened)
            {
                if (currentKingInfo.upLeftBox.x < knightBox.x)
                {
                    addAll = false;
                }
            }
        }
        //if the knight is below the king
        if (knightBox.y > kingBox.y)
        {
            if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downRightThreatened) == KingThreatenedInfo::downRightThreatened)
            {
                if (currentKingInfo.downRightBox.x > knightBox.x)
                {
                    addAll = false;
                }
            }
        }
    }

    //if the knight isn't pinned, we can just add on all the possible knight moves.
    if (addAll)
    {
        calculateKnightPseudoLegalMoves(knightBox, currentBoardState, currentLegalMoves);
    }
}

void Board::calculateKingLegalMoves(Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves, KingThreatenedInfo currentKingInfo)
{
    int x = kingBox.x;
    int y = kingBox.y;
    uint8_t** board = currentBoardState->getBoard();
    uint8_t currentPlayerPieceType = (currentBoardState->getCurrentTurn() == 'w') ? Piece::white : Piece::black;

    for (int change = -1; change <= 1; ++change)
    {
        if (y + 1 < boardYBoxes)
        {
            if (x + change < boardXBoxes && x + change >= 0)
            {
                if ((board[x + change][y + 1] & currentPlayerPieceType) != currentPlayerPieceType)
                {
                    if (!squareAttacked({ x + change,y + 1 }, currentBoardState))
                    {
                        currentLegalMoves.push_back({ { x,y }, { x + change,y + 1 }, false, false, false, false, ' ' });
                    }
                }
            }
        }
        if (y - 1 >= 0)
        {
            if (x + change < boardXBoxes && x + change >= 0)
            {
                if ((board[x + change][y - 1] & currentPlayerPieceType) != currentPlayerPieceType)
                {
                    if (!squareAttacked({ x + change,y - 1 }, currentBoardState))
                    {
                        currentLegalMoves.push_back({ { x,y }, { x + change,y - 1 }, false, false, false, false, ' ' });
                    }
                }
            }
        }
    }
    if (x + 1 < boardXBoxes)
    {
        if (!squareAttacked({ x + 1,y }, currentBoardState))
        {
            if ((board[x + 1][y] & currentPlayerPieceType) != currentPlayerPieceType)
            {
                currentLegalMoves.push_back({ { x,y }, { x + 1,y}, false, false, false, false, ' ' });
            }
        }
    }
    if (x - 1 >= 0)
    {
        if ((board[x - 1][y] & currentPlayerPieceType) != currentPlayerPieceType)
        {
            if (!squareAttacked({ x - 1,y }, currentBoardState))
            {
                currentLegalMoves.push_back({ { x,y }, { x - 1, y}, false, false, false, false, ' ' });
            }
        }
    }
    calculateCastlingLegalMoves(kingBox, currentBoardState, currentLegalMoves);

}

void Board::calculateCastlingLegalMoves(Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves)
{
    int x = kingBox.x;
    int y = kingBox.y;
    uint8_t** board = currentBoardState->getBoard();
    int attackedAmount;
    char currentTurn = currentBoardState->getCurrentTurn();
    if (currentTurn == 'w')
    {
        attackedAmount = whiteThreatened.amountAttacked;
    }
    else
    {
        attackedAmount = blackThreatened.amountAttacked;
    }
    //can't castle if the king is in check.
    if (attackedAmount != 0)
    {
        return;
    }

    //can't castle if the cells in the castling way is attacked
    if ((board[x][y] & Piece::white) == Piece::white)
    {
        if (currentBoardState->getWhiteCanKingsideCastle())
        {
            if (board[x + 1][y] == 0 && board[x + 2][y] == 0)
            {
                if (!squareAttacked({ x + 1,y }, currentBoardState) && !squareAttacked({ x + 2,y }, currentBoardState))
                {
                    currentLegalMoves.push_back({ {x,y},{x + 2,y},true,false,false });
                }
            }
        }
        if (currentBoardState->getWhiteCanQueensideCastle())
        {
            if (board[x - 1][y] == 0 && board[x - 2][y] == 0 && board[x-3][y] == 0)
            {
                if (!squareAttacked({ x - 1,y }, currentBoardState) && !squareAttacked({ x - 2,y }, currentBoardState))
                {
                    currentLegalMoves.push_back({ {x,y},{x - 2,y},false,true,false });
                }
            }
        }
    }
    else
    {
        if (currentBoardState->getBlackCanKingsideCastle())
        {
            if (board[x + 1][y] == 0 && board[x + 2][y] == 0)
            {
                if (!squareAttacked({ x + 1,y }, currentBoardState) && !squareAttacked({ x + 2,y }, currentBoardState))
                {
                    currentLegalMoves.push_back({ {x,y},{x + 2,y},true,false,false });
                }
            }
        }
        if (currentBoardState->getBlackCanQueensideCastle())
        {
            if (board[x - 1][y] == 0 && board[x - 2][y] == 0 && board[x - 3][y] == 0)
            {
                if (!squareAttacked({ x - 1,y }, currentBoardState) && !squareAttacked({ x - 2,y }, currentBoardState))
                {
                    currentLegalMoves.push_back({ {x,y},{x - 2,y},false,true,false });
                }
            }
        }
    }
}

void Board::calculatePawnLegalMoves(Box pawnBox, Box kingBox, BoardState* currentBoardState, std::vector<Move>& currentLegalMoves, KingThreatenedInfo currentKingInfo)
{
    //checking if the pawn is absolute pinned
    char currentTurn = currentBoardState->getCurrentTurn();
    bool addAll = true;

    if (currentTurn == 'w')
    {
        //if the pawn is pinned from above (or below) we only add the pawn moves that are straight up (for white)
        if (inSameCol(pawnBox, kingBox))
        {
            //if the pawn is above the king.
            if (pawnBox.y < kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightUpThreatened) == KingThreatenedInfo::straightUpThreatened)
                {
                    if (currentKingInfo.straightUpBox.y < pawnBox.y)
                    {
                        addStraightUpPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
            //the pawn is below the king.
            else if (pawnBox.y > kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightDownThreatened) == KingThreatenedInfo::straightDownThreatened)
                {
                    if (currentKingInfo.straightDownBox.y > pawnBox.y)
                    {
                        addStraightUpPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
        }
        //if the pawn is pinned on a row we can't move it
        else if (inSameRow(pawnBox, kingBox))
        {
            //pawn is to the right of the king.
            if (pawnBox.x > kingBox.x)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightRightThreatened) == KingThreatenedInfo::straightRightThreatened)
                {
                    if (currentKingInfo.straightRightBox.x > pawnBox.x)
                    {
                        addAll = false;
                    }
                }
            }
            //pawn is to the left of the king.
            else if (pawnBox.x < kingBox.x)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightLeftThreatened) == KingThreatenedInfo::straightLeftThreatened)
                {
                    if (currentKingInfo.straightLeftBox.x < pawnBox.x)
                    {
                        addAll = false;
                    }
                }
            }
        }

        //if the pawn is pinned diagonally (bottom left-top right diagonal), we can only move the pawn (capture) to the top right.
        else if (inSameDiagonal(pawnBox, kingBox))
        {
            //if the pawn is above the king.
            if (pawnBox.y < kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upRightThreatened) == KingThreatenedInfo::upRightThreatened)
                {
                    if (currentKingInfo.upRightBox.x > pawnBox.x)
                    {
                        addUpRightPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
            //if the pawn is below the king
            if (pawnBox.y > kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downLeftThreatened) == KingThreatenedInfo::downLeftThreatened)
                {
                    if (currentKingInfo.downLeftBox.x < pawnBox.x)
                    {
                        addUpRightPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
        }
        //if the pawn is pinned diagonally (top left-bottom right diagonal), we can only move the pawn (capture) to the top left.
        else if (inSameReverseDiagonal(pawnBox, kingBox))
        {
            //if the pawn is above the king.
            if (pawnBox.y < kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upLeftThreatened) == KingThreatenedInfo::upLeftThreatened)
                {
                    if (currentKingInfo.upLeftBox.x < pawnBox.x)
                    {
                        addUpLeftPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
            //if the pawn is below the king
            if (pawnBox.y > kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downRightThreatened) == KingThreatenedInfo::downRightThreatened)
                {
                    if (currentKingInfo.downRightBox.x > pawnBox.x)
                    {
                        addUpLeftPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
        }
        else
        {

        }

        //if the pawn is not pinned, we can just add on all the possible pawn moves.
        if (addAll)
        {
            calculatePawnUpPseudoLegalMoves(pawnBox, currentBoardState, currentLegalMoves);
        }
    }
    //if it's blacks turn.
    else
    {
        //if the pawn is pinned from above (or below) we only add the pawn moves that are straight down (for black)
        if (inSameCol(pawnBox, kingBox))
        {
            //if the pawn is above the king.
            if (pawnBox.y < kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightUpThreatened) == KingThreatenedInfo::straightUpThreatened)
                {
                    if (currentKingInfo.straightUpBox.y < pawnBox.y)
                    {
                        addStraightDownPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
            //the pawn is below the king.
            else if (pawnBox.y > kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightDownThreatened) == KingThreatenedInfo::straightDownThreatened)
                {
                    if (currentKingInfo.straightDownBox.y > pawnBox.y)
                    {
                        addStraightDownPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
        }
        //if the pawn is pinned on a row we can't move it
        else if (inSameRow(pawnBox, kingBox))
        {
            //pawn is to the right of the king.
            if (pawnBox.x > kingBox.x)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightRightThreatened) == KingThreatenedInfo::straightRightThreatened)
                {
                    if (currentKingInfo.straightRightBox.x > pawnBox.x)
                    {
                        addAll = false;
                    }
                }
            }
            else if (pawnBox.x < kingBox.x)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::straightLeftThreatened) == KingThreatenedInfo::straightLeftThreatened)
                {
                    if (currentKingInfo.straightLeftBox.x < pawnBox.x)
                    {
                        addAll = false;
                    }
                }
            }
        }
        //if the pawn is pinned diagonally (bottom left-top right diagonal), we can only move the pawn (capture) to the bottom left.
        else if (inSameDiagonal(pawnBox, kingBox))
        {
            //if the pawn is above the king.
            if (pawnBox.y < kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upRightThreatened) == KingThreatenedInfo::upRightThreatened)
                {
                    if (currentKingInfo.upRightBox.x > pawnBox.x)
                    {
                        addDownLeftPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
            //if the pawn is below the king
            if (pawnBox.y > kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downLeftThreatened) == KingThreatenedInfo::downLeftThreatened)
                {
                    if (currentKingInfo.downLeftBox.x < pawnBox.x)
                    {
                        addDownLeftPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
        }
        //if the pawn is pinned diagonally (top left-bottom right diagonal), we can only move the pawn (capture) to the bottom right.
        else if (inSameReverseDiagonal(pawnBox, kingBox))
        {
            //if the pawn is above the king.
            if (pawnBox.y < kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::upLeftThreatened) == KingThreatenedInfo::upLeftThreatened)
                {
                    if (currentKingInfo.upLeftBox.x < pawnBox.x)
                    {
                        addDownRightPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }

                }
            }
            //if the pawn is below the king
            if (pawnBox.y > kingBox.y)
            {
                if ((currentKingInfo.threatenedInfo & KingThreatenedInfo::downRightThreatened) == KingThreatenedInfo::downRightThreatened)
                {
                    if (currentKingInfo.downRightBox.x > pawnBox.x)
                    {
                        addDownRightPawnMoves(pawnBox, currentBoardState, currentLegalMoves);
                        addAll = false;
                    }
                }
            }
        }

        //if the pawn is not pinned, we can just add on all the possible pawn moves.
        if (addAll)
        {
            calculatePawnDownPseudoLegalMoves(pawnBox, currentBoardState, currentLegalMoves);
        }
    }
}
