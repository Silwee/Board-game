#include "Chess_Board.h"
#include <iostream>


void Board::clearThreats()
{
    whiteThreatened.threatenedInfo = 0;
    whiteThreatened.attackedInfo = 0;

    blackThreatened.threatenedInfo = 0;
    blackThreatened.attackedInfo = 0;

    whiteThreatened.amountAttacked = 0;
    blackThreatened.amountAttacked = 0;
}

void Board::updateAllThreats(char currentTurn, BoardState* currentBoardState)
{
    updateStraightDownThreats(currentTurn, currentBoardState);
    updateStraightUpThreats(currentTurn, currentBoardState);
    updateStraightLeftThreats(currentTurn, currentBoardState);
    updateStraightRightThreats(currentTurn, currentBoardState);
    updateUpLeftThreats(currentTurn, currentBoardState);
    updateUpRightThreats(currentTurn, currentBoardState);
    updateDownLeftThreats(currentTurn, currentBoardState);
    updateDownRightThreats(currentTurn, currentBoardState);
}

void Board::updateMoveToThreats(Move lastMove, BoardState* currentBoardState)
{
    char currentTurn = currentBoardState->getCurrentTurn();
    Box kingBox;
    if (currentTurn == 'w')
    {
        kingBox = whiteLocations.getKingLocation();
    }
    else
    {
        kingBox = blackLocations.getKingLocation();
    }
    uint8_t** board = currentBoardState->getBoard();
    if ((board[lastMove.toBox.x][lastMove.toBox.y] & Piece::king) == Piece::king)
    {
        updateAllThreats(currentTurn, currentBoardState);
    }
    else
    {
        //now we look at where the new piece moved TO.
        if (inSameRow(lastMove.toBox, kingBox))
        {
            if (lastMove.toBox.x > kingBox.x)
            {
                updateStraightRightThreats(currentTurn, currentBoardState);
            }
            else
            {
                updateStraightLeftThreats(currentTurn, currentBoardState);
            }
        }
        else if (inSameCol(lastMove.toBox, kingBox))
        {
            if (lastMove.toBox.y > kingBox.y)
            {
                updateStraightDownThreats(currentTurn, currentBoardState);
            }
            else
            {
                updateStraightUpThreats(currentTurn, currentBoardState);
            }
        }
        else if (inSameDiagonal(lastMove.toBox, kingBox))
        {
            if (lastMove.toBox.x > kingBox.x)
            {
                updateUpRightThreats(currentTurn, currentBoardState);
            }
            else
            {
                updateDownLeftThreats(currentTurn, currentBoardState);
            }
        }
        else if (inSameReverseDiagonal(lastMove.toBox, kingBox))
        {
            if (lastMove.toBox.x > kingBox.x)
            {
                updateDownRightThreats(currentTurn, currentBoardState);
            }
            else
            {
                updateUpLeftThreats(currentTurn, currentBoardState);
            }
        }
    }
}

//updates the current players king threats based on the last move by other player. This is after we switch turns.
void Board::updateThreats(Move lastMove, BoardState* currentBoardState)
{
    //see where they moved from first.
    char currentTurn = currentBoardState->getCurrentTurn();
    Box kingBox;
    if (currentTurn == 'w')
    {
        kingBox = whiteLocations.getKingLocation();
        if ((currentBoardState->getBoard()[lastMove.toBox.x][lastMove.toBox.y]) == (Piece::king | Piece::white))
        {
            updateAllThreats(currentTurn, currentBoardState);
            return;
        }
    }
    else
    {
        kingBox = blackLocations.getKingLocation();
        if ((currentBoardState->getBoard()[lastMove.toBox.x][lastMove.toBox.y]) == (Piece::king | Piece::black))
        {
            updateAllThreats(currentTurn, currentBoardState);
            return;
        }
    }

    if (currentTurn == 'b')
    {
        if(currentBoardState->getBoard()[lastMove.toBox.x][lastMove.toBox.y] == (Piece::white | Piece::knight))
        {

            int xDiff = abs(lastMove.toBox.x - kingBox.x);
            int yDiff = abs(lastMove.toBox.y - kingBox.y);
            int sumOfLeftAndRight = xDiff + yDiff;
            //that means there's a square thats 2-1 between them.
            if (sumOfLeftAndRight == 3 && xDiff != 0 && yDiff != 0)
            {
                blackThreatened.amountAttacked++;
                blackThreatened.attackedByKnight = true;
                blackThreatened.attackedFromBox = { lastMove.toBox.x,lastMove.toBox.y };
            }
        } //IF THE LAST MOVE WAS A PAWN
        else if (currentBoardState->getBoard()[lastMove.toBox.x][lastMove.toBox.y] == (Piece::white | Piece::pawn))
        {
            if (lastMove.toBox.x == kingBox.x + 1)
            {
                //if it's in one of the diagonal rows
                if (lastMove.toBox.y == kingBox.y + 1)
                {
                    blackThreatened.amountAttacked++;
                    blackThreatened.threatenedInfo |= KingThreatenedInfo::downRightThreatened;
                    blackThreatened.attackedInfo |= KingThreatenedInfo::downRightThreatened;
                    blackThreatened.attackedFromBox = { lastMove.toBox.x,lastMove.toBox.y };
                }
            }
            else if (lastMove.toBox.x == kingBox.x - 1)
            {
                if (lastMove.toBox.y == kingBox.y + 1)
                {
                    blackThreatened.amountAttacked++;
                    blackThreatened.threatenedInfo |= KingThreatenedInfo::downLeftThreatened;
                    blackThreatened.attackedInfo |= KingThreatenedInfo::downLeftThreatened;
                    blackThreatened.attackedFromBox = { lastMove.toBox.x,lastMove.toBox.y };
                }
            }
        }
    }
    else
    {
        if (currentBoardState->getBoard()[lastMove.toBox.x][lastMove.toBox.y] == (Piece::black | Piece::knight))
        {
            int xDiff = abs(lastMove.toBox.x - kingBox.x);
            int yDiff = abs(lastMove.toBox.y - kingBox.y);
            int sumOfLeftAndRight = xDiff + yDiff;
            //that means there's a square thats 2-1 between them.
            if (sumOfLeftAndRight == 3 && xDiff != 0 && yDiff != 0)
            {
                whiteThreatened.amountAttacked++;
                whiteThreatened.attackedByKnight = true;
                whiteThreatened.attackedFromBox = { lastMove.toBox.x,lastMove.toBox.y };
            }
        }
        else if (currentBoardState->getBoard()[lastMove.toBox.x][lastMove.toBox.y] == (Piece::black | Piece::pawn))
        {
            if (lastMove.toBox.x == kingBox.x + 1)
            {
                //if it's in one of the diagonal rows
                if (lastMove.toBox.y == kingBox.y - 1)
                {
                    whiteThreatened.amountAttacked++;
                    whiteThreatened.threatenedInfo |= KingThreatenedInfo::upRightThreatened;
                    whiteThreatened.attackedInfo |= KingThreatenedInfo::upRightThreatened;
                    whiteThreatened.attackedFromBox = { lastMove.toBox.x,lastMove.toBox.y };
                }
            }
            else if (lastMove.toBox.x == kingBox.x - 1)
            {
                if (lastMove.toBox.y == kingBox.y - 1)
                {
                    whiteThreatened.amountAttacked++;
                    whiteThreatened.threatenedInfo |= KingThreatenedInfo::upLeftThreatened;
                    whiteThreatened.attackedInfo |= KingThreatenedInfo::upLeftThreatened;
                    whiteThreatened.attackedFromBox = { lastMove.toBox.x,lastMove.toBox.y };
                }
            }
        }
    }

    //if we castled just update for the rook. there's a better way to do this but have you seen the rest of my code?
    if (lastMove.kingSideCastle || lastMove.queenSideCastle)
    {
        updateStraightDownThreats(currentTurn, currentBoardState);
        updateStraightUpThreats(currentTurn, currentBoardState);
        updateStraightLeftThreats(currentTurn, currentBoardState);
        updateStraightRightThreats(currentTurn, currentBoardState);
    }
    if (inSameRow(kingBox, lastMove.toBox))
    {
        //std::cout << "Last move was from same row as king. " << std::endl;
        if (kingBox.x < lastMove.toBox.x)
        {
            //std::cout << "Updating right threats" << std::endl;
            updateStraightRightThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateStraightLeftThreats(currentTurn, currentBoardState);
        }
    }
    else if (inSameCol(kingBox, lastMove.toBox))
    {

        if (kingBox.y < lastMove.toBox.y)
        {
            updateStraightDownThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateStraightUpThreats(currentTurn, currentBoardState);
        }
    }
    else if (inSameDiagonal(kingBox, lastMove.toBox))
    {
        if (kingBox.x < lastMove.toBox.x)
        {
            updateUpRightThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateDownLeftThreats(currentTurn, currentBoardState);
        }
    }
    else if (inSameReverseDiagonal(kingBox, lastMove.toBox))
    {
        if (kingBox.x < lastMove.toBox.x)
        {
            updateDownRightThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateUpLeftThreats(currentTurn, currentBoardState);
        }
    }

    //now we look at where the new piece moved TO.
    if (inSameRow(lastMove.fromBox, kingBox) && !inSameRow(lastMove.fromBox, lastMove.toBox))
    {
        if (lastMove.fromBox.x > kingBox.x)
        {
            updateStraightRightThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateStraightLeftThreats(currentTurn, currentBoardState);
        }
    }
    else if (inSameCol(lastMove.fromBox, kingBox) && !inSameCol(lastMove.fromBox, lastMove.toBox))
    {
        if (lastMove.fromBox.y > kingBox.y)
        {
            updateStraightDownThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateStraightUpThreats(currentTurn, currentBoardState);
        }
    }
    else if (inSameDiagonal(lastMove.fromBox, kingBox) && !inSameDiagonal(lastMove.fromBox, lastMove.toBox))
    {
        if (lastMove.fromBox.x > kingBox.x)
        {
            updateUpRightThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateDownLeftThreats(currentTurn, currentBoardState);
        }
    }
    else if (inSameReverseDiagonal(lastMove.fromBox, kingBox) && !inSameReverseDiagonal(lastMove.fromBox, lastMove.toBox))
    {
        if (lastMove.fromBox.x > kingBox.x)
        {
            updateDownRightThreats(currentTurn, currentBoardState);
        }
        else
        {
            updateUpLeftThreats(currentTurn, currentBoardState);
        }
    }
}


void Board::initializeKingsThreatened(BoardState* currentBoardState)
{
    //std::cout << "Threatened at beginning of initialize: " << int(blackThreatened.threatenedInfo) << std::endl;
    whiteThreatened.threatenedInfo = 0;
    blackThreatened.threatenedInfo = 0;

    updateStraightUpThreats('w', currentBoardState);
    updateStraightDownThreats('w', currentBoardState);
    updateStraightLeftThreats('w', currentBoardState);
    updateStraightRightThreats('w', currentBoardState);

    updateDownLeftThreats('w', currentBoardState);
    updateUpLeftThreats('w', currentBoardState);
    updateDownRightThreats('w', currentBoardState);
    updateUpRightThreats('w', currentBoardState);

    updateStraightUpThreats('b', currentBoardState);
    updateStraightDownThreats('b', currentBoardState);
    updateStraightLeftThreats('b', currentBoardState);
    updateStraightRightThreats('b', currentBoardState);

    updateDownLeftThreats('b', currentBoardState);
    updateUpLeftThreats('b', currentBoardState);
    updateDownRightThreats('b', currentBoardState);
    updateUpRightThreats('b', currentBoardState);
    //std::cout << "Threatened at end of initialize: " << int(blackThreatened.threatenedInfo) << std::endl;
}


void Board::updateStraightUpThreats(char currentTurn, BoardState* currentBoardState)
{
    int defense = 0;
    uint8_t** board = currentBoardState->getBoard();
    Box kingSquare;
    bool foundAThreat = false;
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        //up
        for (int y = kingSquare.y - 1; y >= 0; --y)
        {
            if (board[kingSquare.x][y] == 0)
            {
                continue;
            }
            else if ((board[kingSquare.x][y] & Piece::white) == Piece::white || ((board[kingSquare.x][y] & Piece::rook) != Piece::rook && (board[kingSquare.x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            else
            {
                if ((board[kingSquare.x][y] & Piece::rook) == Piece::rook || (board[kingSquare.x][y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.straightUpBox = { kingSquare.x,y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::straightUpThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightUpThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { kingSquare.x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightUpThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 2);
        }

    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        //up
        for (int y = kingSquare.y - 1; y >= 0; --y)
        {
            if (board[kingSquare.x][y] == 0)
            {
                continue;
            }
            else if ((board[kingSquare.x][y] & Piece::black) == Piece::black || ((board[kingSquare.x][y] & Piece::rook) != Piece::rook && (board[kingSquare.x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[kingSquare.x][y] & Piece::rook) == Piece::rook || (board[kingSquare.x][y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.straightUpBox = { kingSquare.x,y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::straightUpThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightUpThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { kingSquare.x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightUpThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 2);
        }
    }
}

void Board::updateStraightDownThreats(char currentTurn, BoardState* currentBoardState)
{
    bool foundAThreat = false;
    Box kingSquare;
    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    //down.
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        for (int y = kingSquare.y + 1; y < boardYBoxes; ++y)
        {
            if (board[kingSquare.x][y] == 0)
            {
                continue;
            }
            else if ((board[kingSquare.x][y] & Piece::white) == Piece::white || ((board[kingSquare.x][y] & Piece::rook) != Piece::rook && (board[kingSquare.x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[kingSquare.x][y] & Piece::rook) == Piece::rook || (board[kingSquare.x][y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.straightDownBox = { kingSquare.x,y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::straightDownThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightDownThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { kingSquare.x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightDownThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 6);
        }
        foundAThreat = false;
    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        for (int y = kingSquare.y + 1; y < boardYBoxes; ++y)
        {
            if (board[kingSquare.x][y] == 0)
            {
                continue;
            }
            else if ((board[kingSquare.x][y] & Piece::black) == Piece::black || ((board[kingSquare.x][y] & Piece::rook) != Piece::rook && (board[kingSquare.x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[kingSquare.x][y] & Piece::rook) == Piece::rook || (board[kingSquare.x][y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.straightDownBox = { kingSquare.x,y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::straightDownThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightDownThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { kingSquare.x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightDownThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 6);
        }
    }

}

void Board::updateStraightLeftThreats(char currentTurn, BoardState* currentBoardState)
{
    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    bool foundAThreat = false;
    Box kingSquare;
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        for (int x = kingSquare.x - 1; x >= 0; --x)
        {
            if (board[x][kingSquare.y] == 0)
            {
                continue;
            }
            else if ((board[x][kingSquare.y] & Piece::white) == Piece::white || ((board[x][kingSquare.y] & Piece::rook) != Piece::rook && (board[x][kingSquare.y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][kingSquare.y] & Piece::rook) == Piece::rook || (board[x][kingSquare.y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.straightLeftBox = { x,kingSquare.y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::straightLeftThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightLeftThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { x,kingSquare.y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightLeftThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 0);
        }
    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        for (int x = kingSquare.x - 1; x >= 0; --x)
        {
            if (board[x][kingSquare.y] == 0)
            {
                continue;
            }
            else if ((board[x][kingSquare.y] & Piece::black) == Piece::black || ((board[x][kingSquare.y] & Piece::rook) != Piece::rook && (board[x][kingSquare.y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][kingSquare.y] & Piece::rook) == Piece::rook || (board[x][kingSquare.y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.straightLeftBox = { x,kingSquare.y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::straightLeftThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightLeftThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { x,kingSquare.y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightLeftThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        //to the left
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 0);
        }
    }

}

void Board::updateStraightRightThreats(char currentTurn, BoardState* currentBoardState)
{

    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    Box kingSquare;
    bool foundAThreat = false;
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        //black time!!!
        defense = 0;

        for (int x = kingSquare.x + 1; x < boardXBoxes; ++x)
        {
            if (board[x][kingSquare.y] == 0)
            {
                continue;
            }
            else if ((board[x][kingSquare.y] & Piece::white) == Piece::white || ((board[x][kingSquare.y] & Piece::rook) != Piece::rook && (board[x][kingSquare.y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {

                    break;
                }
            }
            //it's a black piece.
            else
            {
                if ((board[x][kingSquare.y] & Piece::rook) == Piece::rook || (board[x][kingSquare.y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.straightRightBox = { x,kingSquare.y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::straightRightThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightRightThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { x,kingSquare.y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::straightRightThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            //std::cout << "white threatened before setting it to 0: " << int(whiteThreatened.threatenedInfo) << std::endl;
            whiteThreatened.threatenedInfo &= ~(1UL << 4);
            //std::cout << "white threatened after setting it to 0: " << int(whiteThreatened.threatenedInfo) << std::endl;
        }
        foundAThreat = false;
    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        //black time!!!
        defense = 0;

        for (int x = kingSquare.x + 1; x < boardXBoxes; ++x)
        {
            if (board[x][kingSquare.y] == 0)
            {
                continue;
            }
            else if ((board[x][kingSquare.y] & Piece::black) == Piece::black || ((board[x][kingSquare.y] & Piece::rook) != Piece::rook && (board[x][kingSquare.y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][kingSquare.y] & Piece::rook) == Piece::rook || (board[x][kingSquare.y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.straightRightBox = { x,kingSquare.y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::straightRightThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightRightThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { x,kingSquare.y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::straightRightThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 4);
        }
    }
}

void Board::updateUpLeftThreats(char currentTurn, BoardState* currentBoardState)
{
    //std::cout << "Checking for up lefts" << std::endl;
    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    bool foundAThreat = false;
    Box kingSquare;
    int x, y;
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (--x >= 0 && --y >= 0)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::white) == Piece::white || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.upLeftBox = { x,y };

                    if (defense == 0)
                    {

                        whiteThreatened.attackedInfo |= KingThreatenedInfo::upLeftThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::upLeftThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::upLeftThreatened;

                        foundAThreat = true;
                        break;
                    }
                }
            }
        }

        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 1);
        }

    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        //up to the left
        x = kingSquare.x;
        y = kingSquare.y;
        while (--x >= 0 && --y >= 0)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::black) == Piece::black || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a black piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.upLeftBox = { x,y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::upLeftThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::upLeftThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::upLeftThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 1);
        }
    }
}

void Board::updateUpRightThreats(char currentTurn, BoardState* currentBoardState)
{
    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    Box kingSquare;
    int x, y;
    bool foundAThreat = false;
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (++x < boardXBoxes && --y >= 0)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::white) == Piece::white || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a black piece
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.upRightBox = { x,y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::upRightThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::upRightThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::upRightThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 3);
        }
        foundAThreat = false;
    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (++x < boardXBoxes && --y >= 0)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::black) == Piece::black || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.upRightBox = { x,y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::upRightThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::upRightThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::upRightThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 3);
        }
    }
}

void Board::updateDownLeftThreats(char currentTurn, BoardState* currentBoardState)
{
    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    Box kingSquare;
    bool foundAThreat = false;
    int x, y;
    if (currentTurn == 'b')
    {
        //down to the left
        kingSquare = blackLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (--x >= 0 && ++y < boardYBoxes)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::black) == Piece::black || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {

                    blackThreatened.downLeftBox = { x,y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::downLeftThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::downLeftThreatened;

                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {

                        blackThreatened.threatenedInfo |= KingThreatenedInfo::downLeftThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 7);
        }
        foundAThreat = false;
    }
    else
    {
        //down to the left
        kingSquare = whiteLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (--x >= 0 && ++y < boardYBoxes)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::white) == Piece::white || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.downLeftBox = { x,y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::downLeftThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::downLeftThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::downLeftThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 7);
        }
    }
}

void Board::updateDownRightThreats(char currentTurn, BoardState* currentBoardState)
{
    uint8_t** board = currentBoardState->getBoard();
    int defense = 0;
    Box kingSquare;
    int x, y;
    bool foundAThreat = false;
    if (currentTurn == 'w')
    {
        kingSquare = whiteLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (++x < boardXBoxes && ++y < boardYBoxes)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::white) == Piece::white || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    whiteThreatened.downRightBox = { x,y };
                    if (defense == 0)
                    {
                        whiteThreatened.attackedInfo |= KingThreatenedInfo::downRightThreatened;
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::downRightThreatened;
                        whiteThreatened.amountAttacked++;
                        whiteThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        whiteThreatened.threatenedInfo |= KingThreatenedInfo::downRightThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }
        }
        if (!foundAThreat)
        {
            whiteThreatened.threatenedInfo &= ~(1UL << 5);
        }
        foundAThreat = false;
    }
    else
    {
        kingSquare = blackLocations.getKingLocation();
        x = kingSquare.x;
        y = kingSquare.y;
        while (++x < boardXBoxes && ++y < boardYBoxes)
        {
            if (board[x][y] == 0)
            {
                continue;
            }
            else if ((board[x][y] & Piece::black) == Piece::black || ((board[x][y] & Piece::bishop) != Piece::bishop && (board[x][y] & Piece::queen) != Piece::queen))
            {
                defense++;
                if (defense == 2)
                {
                    break;
                }
            }
            //it's a white piece.
            else
            {
                if ((board[x][y] & Piece::bishop) == Piece::bishop || (board[x][y] & Piece::queen) == Piece::queen)
                {
                    blackThreatened.downRightBox = { x,y };
                    if (defense == 0)
                    {
                        blackThreatened.attackedInfo |= KingThreatenedInfo::downRightThreatened;
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::downRightThreatened;
                        blackThreatened.amountAttacked++;
                        blackThreatened.attackedFromBox = { x,y };
                        foundAThreat = true;
                        break;
                    }
                    else
                    {
                        blackThreatened.threatenedInfo |= KingThreatenedInfo::downRightThreatened;
                        foundAThreat = true;
                        break;
                    }
                }
            }

        }
        if (!foundAThreat)
        {
            blackThreatened.threatenedInfo &= ~(1UL << 5);
        }
    }
}
