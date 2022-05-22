#include "Chess_Board.h"
#include <iostream>

const uint8_t KingThreatenedInfo::straightLeftThreatened = 0b00000001;
const uint8_t KingThreatenedInfo::upLeftThreatened = 0b00000010;
const uint8_t KingThreatenedInfo::straightUpThreatened = 0b00000100;
const uint8_t KingThreatenedInfo::upRightThreatened = 0b00001000;
const uint8_t KingThreatenedInfo::straightRightThreatened = 0b00010000;
const uint8_t KingThreatenedInfo::downRightThreatened = 0b00100000;
const uint8_t KingThreatenedInfo::straightDownThreatened = 0b01000000;
const uint8_t KingThreatenedInfo::downLeftThreatened = 0b10000000;

BoardState* Board::getBoardState()
{
    return boardState;
}

int Board::getHeight()
{
    return boardYBoxes * boxYHeight;
}

int Board::getWidth()
{
    return boardXBoxes * boxXWidth;
}

//for debug purpose
void Board::printBoardState(BoardState* currentBoardState)
{
    if (currentBoardState->getCurrentTurn() == 'w')
    {
        std::cout << "White turn." << std::endl;
    }
    else
    {
        std::cout << "Black turn." << std::endl;
    }
    if (currentBoardState->getWhiteCanKingsideCastle())
    {
        std::cout << "White can kingside castle." << std::endl;
    }
    else
    {
        std::cout << "White cannot kingside castle." << std::endl;
    }
    if (currentBoardState->getWhiteCanQueensideCastle())
    {
        std::cout << "White can queenside castle." << std::endl;
    }
    else
    {
        std::cout << "White cannot queenside castle." << std::endl;
    }
    if (currentBoardState->getBlackCanKingsideCastle())
    {
        std::cout << "Black can kingside castle." << std::endl;
    }
    else
    {
        std::cout << "Black cannot kingside castle." << std::endl;
    }
    if (currentBoardState->getBlackCanQueensideCastle())
    {
        std::cout << "Black can Queenside castle." << std::endl;
    }
    else
    {
        std::cout << "Black cannot Queenside castle." << std::endl;
    }
    if (currentBoardState->getEnPassantX() != -1)
    {
        std::cout << "En passant opportunity at x: " << boardState->getEnPassantX() << " and y: " << boardState->getEnPassantY() << std::endl;
    }
    else
    {
        std::cout << "No french move" << std::endl;
    }

    std::cout << "Num of ply:" << currentBoardState->getHalfMoveClock() << std::endl;
    std::cout << "Num of move:" << currentBoardState->getFullMoveClock() << std::endl;
}


bool Board::inLegalMoves(struct Move& newMove)
{
    for (unsigned i = 0; i < legalMoves.size(); i++)
    {
        if (newMove.toBox == legalMoves[i].toBox && newMove.fromBox == legalMoves[i].fromBox)
        {
            newMove.kingSideCastle = legalMoves[i].kingSideCastle;
            newMove.queenSideCastle = legalMoves[i].queenSideCastle;
            newMove.enPassant = legalMoves[i].enPassant;
            newMove.isPromotion = legalMoves[i].isPromotion;
            newMove.promotionType = legalMoves[i].promotionType;

            return true;
        }
    }
    return false;
}

bool Board::isEnPassant(Box fromBox, Box toBox, BoardState* currentBoardState)
{
    if ((currentBoardState->getBoard()[fromBox.x][fromBox.y] & Piece::pawn) == Piece::pawn)
    {
        if (toBox.x == currentBoardState->getEnPassantX() && toBox.y == currentBoardState->getEnPassantY())
        {
            return true;
        }
    }

    return false;
}

bool Board::canEnPassant(Box pawnBox, Box toBox, BoardState* currentBoardState)
{
    char currentTurn = currentBoardState->getCurrentTurn();
    Box kingBox;
    uint8_t enemyPiece;
    uint8_t** board = currentBoardState->getBoard();
    if (currentTurn == 'w')
    {
        kingBox = whiteLocations.getKingLocation();
        enemyPiece = Piece::black;
    }
    else
    {
        kingBox = blackLocations.getKingLocation();
        enemyPiece = Piece::white;
    }

    if (inSameRow(kingBox, { pawnBox.x,pawnBox.y }))
    {
        //if the pawn is on the right of the king
        if (pawnBox.x > kingBox.x)
        {
            //enPassanting to the right, therefore taking a pawn away from pawnBox.x + 1
            if (toBox.x > pawnBox.x)
            {
                for (int i = pawnBox.x+2; i < boardXBoxes; ++i)
                {
                    if (board[i][pawnBox.y] == 0)
                    {
                        continue;
                    }
                    else if ((board[i][pawnBox.y] == (enemyPiece | Piece::rook)) || (board[i][pawnBox.y] == (enemyPiece | Piece::queen)))
                    {
                        for (int j = pawnBox.x - 1; j > kingBox.x; --j)
                        {
                            if (board[j][pawnBox.y] != 0)
                            {
                                return true;
                            }
                        }
                        return false;
                    }
                    else  	//if there's some other piece there.
                    {
                        break;
                    }
                }
            }
            //enPassanting to the left, therefore taking a pawn away from pawnBox.x -1 but we still search from the right side
            else
            {
                for (int i = pawnBox.x +1; i < boardXBoxes; ++i)
                {
                    if (board[i][pawnBox.y] == 0)
                    {
                        continue;
                    }
                    else if ((board[i][pawnBox.y] == (enemyPiece | Piece::rook)) || (board[i][pawnBox.y] == (enemyPiece | Piece::queen)))
                    {
                        for (int j = pawnBox.x - 2; j > kingBox.x; --j)
                        {
                            if (board[j][pawnBox.y] != 0)
                            {
                                return true;
                            }
                        }
                        return false;
                    }
                    else  	//if there's some other piece there.
                    {
                        break;
                    }
                }
            }

        }
        else
        {
            //en passanting to right on the left side of the king
            if (toBox.x > pawnBox.x)
            {
                for (int i = pawnBox.x - 1; i >=0; --i)
                {
                    if (board[i][pawnBox.y] == 0)
                    {
                        continue;
                    }
                    else if ((board[i][pawnBox.y] == (enemyPiece | Piece::rook)) || (board[i][pawnBox.y] == (enemyPiece | Piece::queen)))
                    {
                        for (int j = pawnBox.x + 2; j < kingBox.x; j++)
                        {
                            if (board[j][pawnBox.y] != 0)
                            {
                                return true;
                            }
                        }
                        return false;
                    }
                    else  	//if there's some other piece there.
                    {
                        break;
                    }
                }
            }
            //en passanting to the left on the left side of the king.
            else
            {
                for (int i = pawnBox.x - 2; i >=0; --i)
                {
                    if (board[i][pawnBox.y] == 0)
                    {
                        continue;
                    }
                    else if ((board[i][pawnBox.y] == (enemyPiece | Piece::rook)) || (board[i][pawnBox.y] == (enemyPiece | Piece::queen)))
                    {
                        for (int j = pawnBox.x + 1; j < kingBox.x; j++)
                        {
                            if (board[j][pawnBox.y] != 0)
                            {
                                return true;
                            }
                        }
                        return false;
                    }
                    else  	//if there's some other piece there.
                    {
                        break;
                    }
                }
            }

        }

    }
    return true;
}

void Board::findKingLocation(Box* box, BoardState* currentBoardState)
{
    char currentPlayer = currentBoardState->getCurrentTurn();
    Box kingBox;
    if (currentPlayer == 'w')
    {
        kingBox = whiteLocations.getKingLocation();
    }
    else
    {
        kingBox = blackLocations.getKingLocation();
    }
    *box = kingBox;
}

bool Board::kingInCheck(BoardState* currentBoardState)
{
    //if the current players king is under attack then the king is in check.
    Box king;
    findKingLocation(&king, currentBoardState);

    bool result = squareAttacked(king, currentBoardState);

    return result;
}

int Board::isGameOver(BoardState* currentBoardState)
{
    //if there are only 2 kings and knights and/or bishops, it maybe a draw
    if (!whiteLocations.numOfPawn() && !whiteLocations.numOfQueen() && !whiteLocations.numOfRook() &&
        !blackLocations.numOfPawn() && !blackLocations.numOfQueen() && !blackLocations.numOfRook())
    {
        //each side have no more than 1 knight or bishop
        if (whiteLocations.numOfKnight() + whiteLocations.numOfBishop() <= 1 &&
            blackLocations.numOfBishop() + blackLocations.numOfKnight() <= 1)
        {
            winner = 'd';
            return 2;
        }
        //2 knight cannot checkmate
        else if (whiteLocations.numOfKnight() == 2 && !whiteLocations.numOfBishop())
        {
            if (!blackLocations.numOfBishop() && !blackLocations.numOfKnight())
            {
                winner = 'd';
                return 2;
            }
        }
        else if (blackLocations.numOfKnight() == 2 && !blackLocations.numOfBishop())
        {
            if (!whiteLocations.numOfBishop() && !whiteLocations.numOfKnight())
            {
                winner = 'd';
                return 2;
            }
        }
        //2 bishop can only checkmate the king if they're in different cell color
        else if (whiteLocations.numOfBishop() == 2 && !whiteLocations.numOfKnight())
        {
            if (!blackLocations.numOfBishop() && !blackLocations.numOfKnight())
            {
                Box firstBishop = whiteLocations.getBishopLocations()[0];
                Box secondBishop = whiteLocations.getBishopLocations()[1];
                if ((firstBishop.x + firstBishop.y) % 2 == (secondBishop.x + secondBishop.y) % 2)
                {
                    winner = 'd';
                    return 2;
                }
            }
        }
        else if (blackLocations.numOfBishop() == 2 && !blackLocations.numOfKnight())
        {
            if (!whiteLocations.numOfBishop() && !whiteLocations.numOfKnight())
            {
                Box firstBishop = blackLocations.getBishopLocations()[0];
                Box secondBishop = blackLocations.getBishopLocations()[1];
                if ((firstBishop.x + firstBishop.y) % 2 == (secondBishop.x + secondBishop.y) % 2)
                {
                    winner = 'd';
                    return 2;
                }
            }
        }
    }
    //if there is no more move
    if (legalMoves.size() == 0)
    {
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

        //if the king is not in check but there is no legal moves, that is a stalemate
        if (attackedAmount != 0)
        {
            winner = (currentBoardState->getCurrentTurn() == 'w' ? 'b' : 'w');
            return 1;
        }
        //checkmate
        else
        {
            winner = 'd';
            return 2;
        }
    }
    if(currentBoardState->getHalfMoveClock() >= 50)
    {
        winner = 'd';
        return 2;
    }
    return 0;
}

bool Board::pieceIsCurrentPlayersPiece(int x, int y, BoardState* currentBoardState)
{
    char currentTurn = currentBoardState->getCurrentTurn();
    uint8_t** board = currentBoardState->getBoard();
    if ((currentTurn == 'w' && (board[x][y] & Piece::white) == Piece::white)
        || (currentTurn == 'b' && (board[x][y] & Piece::black) == Piece::black))
    {
        return true;
    }
    return false;
}

bool Board::kingAttacked(BoardState* currentBoardState)
{
    char currentTurn = currentBoardState->getCurrentTurn();
    if (currentTurn == 'w')
    {
        return whiteThreatened.amountAttacked >= 1;
    }
    return blackThreatened.amountAttacked >= 1;
}


bool Board::doesBoxBlockAttack(Box box, BoardState* currentBoardState)
{
    char currentTurn = currentBoardState->getCurrentTurn();

    uint8_t attackedInfo;
    Box attackedFromBox;
    Box kingBox;
    bool attackedByKnight;
    if (currentTurn == 'w')
    {
        attackedInfo = whiteThreatened.attackedInfo;
        attackedFromBox = { whiteThreatened.attackedFromBox.x,whiteThreatened.attackedFromBox.y };
        attackedByKnight = whiteThreatened.attackedByKnight;
        kingBox = { whiteLocations.getKingLocation().x,whiteLocations.getKingLocation().y };
    }
    else
    {
        attackedInfo = blackThreatened.attackedInfo;
        attackedFromBox = { blackThreatened.attackedFromBox.x,blackThreatened.attackedFromBox.y };
        attackedByKnight = blackThreatened.attackedByKnight;
        kingBox = { blackLocations.getKingLocation().x,blackLocations.getKingLocation().y };
    }

    //when you're attacked by a knight the only way to block it with a piece is to take it.
    if (attackedByKnight)
    {
        return box.x == attackedFromBox.x && box.y == attackedFromBox.y;
    }
    if (box.x == attackedFromBox.x && box.y == attackedFromBox.y)
    {
        return true;
    }


    switch (attackedInfo)
    {
    case KingThreatenedInfo::straightDownThreatened:
        return box.x == attackedFromBox.x && box.y < attackedFromBox.y && box.y > kingBox.y;
        break;
    case KingThreatenedInfo::straightUpThreatened:
        return box.x == attackedFromBox.x && box.y > attackedFromBox.y && box.y < kingBox.y;
        break;
    case KingThreatenedInfo::straightLeftThreatened:
        return box.y == attackedFromBox.y && box.x > attackedFromBox.x && box.x < kingBox.x;
        break;
    case KingThreatenedInfo::straightRightThreatened:
        return box.y == attackedFromBox.y && box.x < attackedFromBox.x && box.x > kingBox.x;
        break;
    case KingThreatenedInfo::upLeftThreatened:
        if (inSameReverseDiagonal(box, kingBox))
        {
            return box.x < kingBox.x && box.x > attackedFromBox.x;
        }
        break;
    case KingThreatenedInfo::upRightThreatened:
        if (inSameDiagonal(box, kingBox))
        {
            return box.x > kingBox.x && box.x < attackedFromBox.x;
        }
        break;
    case KingThreatenedInfo::downLeftThreatened:
        if (inSameDiagonal(box, kingBox))
        {
            return box.x < kingBox.x && box.x > attackedFromBox.x;
        }
        break;
    case KingThreatenedInfo::downRightThreatened:
        if (inSameReverseDiagonal(box, kingBox))
        {
            return box.x > kingBox.x && box.x < attackedFromBox.x;
        }
        break;
    default:
        std::cout << "error in switch" << std::endl;
    }
    return false;
}


bool Board::squareAttacked(Box box, BoardState* currentBoardState)
{
    int x = box.x;
    int y = box.y;
    char currentTurn = currentBoardState->getCurrentTurn();
    uint8_t** board = currentBoardState->getBoard();
    //pawns attack differently than they move.
    if (currentTurn == 'b')
    {
        if (y + 1 < boardYBoxes)
        {
            if (x - 1 >= 0)
            {
                if ((board[x - 1][y + 1]) == (Piece::pawn | Piece::white))
                {
                    return true;
                }
            }
            if (x + 1 < boardXBoxes)
            {
                if (board[x + 1][y + 1] == (Piece::pawn | Piece::white))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if (y - 1 >= 0)
        {
            if (x - 1 >= 0)
            {
                if (board[x - 1][y - 1] == (Piece::pawn | Piece::black))
                {
                    return true;
                }
            }
            if (x + 1 < boardXBoxes)
            {
                if (board[x + 1][y - 1] == (Piece::pawn | Piece::black))
                {
                    return true;
                }
            }
        }
    }

    uint8_t yourColor;
    uint8_t enemyColor;
    if (currentTurn == 'w')
    {
        enemyColor = Piece::black;
        yourColor = Piece::white;
    }
    else
    {
        enemyColor = Piece::white;
        yourColor = Piece::black;
    }

    //going to right
    if (x + 1 < boardXBoxes)
    {
        if (board[x + 1][y] == (enemyColor | Piece::king))
        {
            return true;
        }
    }

    for (int currX = x + 1; currX < boardXBoxes; ++currX)
    {
        if (board[currX][y] == 0)
        {
            continue;
        }
        //if it's an enemy piece.
        else if (board[currX][y] == (enemyColor | Piece::queen) || board[currX][y] == (enemyColor | Piece::rook))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if ((board[currX][y] ) != (Piece::king | yourColor))
            {
                break;
            }
        }
    }

    if (x - 1 >= 0)
    {
        if (board[x - 1][y] == (enemyColor | Piece::king))
        {
            return true;
        }
    }

    //going to left
    for (int currX = x - 1; currX >= 0; --currX)
    {
        if (board[currX][y] == 0)
        {
            continue;
        }//if it's an enemy piece.
        else if (board[currX][y] == (enemyColor | Piece::queen) || board[currX][y] == (enemyColor | Piece::rook))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[currX][y] != (Piece::king | yourColor))
            {
                break;
            }
        }
    }

    if (y - 1 >= 0)
    {
        if (board[x][y - 1] == (enemyColor | Piece::king))
        {
            return true;
        }
    }

    //going  up
    for (int currY = y - 1; currY >= 0; --currY)
    {
        if (board[x][currY] == 0)
        {
            continue;
        }//if it's an enemy piece.
        else if (board[x][currY] == (enemyColor | Piece::queen) || board[x][currY] == (enemyColor | Piece::rook))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[x][currY] != (Piece::king | yourColor))
            {
                break;
            }
        }
    }
    //going down
    if (y + 1 < boardYBoxes)
    {
        if (board[x][y + 1] == (enemyColor | Piece::king))
        {
            return true;
        }
    }

    for (int currY = y + 1; currY < boardYBoxes; ++currY)
    {
        if (board[x][currY] == 0)
        {
            continue;
        }//if it's an enemy piece.
        else if (board[x][currY] == (enemyColor | Piece::queen) || board[x][currY] == (enemyColor | Piece::rook))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[x][currY] != (Piece::king | yourColor))
            {
                break;
            }
        }
    }

    int increase = 1;
    //going down to right
    if (x + increase < boardXBoxes && y + increase < boardYBoxes)
    {
        if (board[x + increase][y + increase] == (enemyColor | Piece::king))
        {
            return true;
        }
    }

    while (x + increase < boardXBoxes && y + increase < boardYBoxes)
    {
        if (board[x + increase][y + increase] == 0)
        {
            ++increase;
            continue;
        }//if it's an enemy piece.
        else if (board[x + increase][y + increase] == (enemyColor | Piece::queen) || board[x + increase][y + increase] == (enemyColor | Piece::bishop))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[x+increase][y+increase] != (Piece::king | yourColor))
            {
                break;
            }
        }
        ++increase;
    }
    increase = 1;

    if (x - increase >= 0 && y + increase < boardYBoxes)
    {
        if (board[x - increase][y + increase] == (enemyColor | Piece::king))
        {
            return true;
        }
    }

    //going down to left
    while (x - increase >= 0 && y + increase < boardYBoxes)
    {
        if (board[x - increase][y + increase] == 0)
        {
            ++increase;
            continue;
        }//if it's an enemy piece.
        else if (board[x - increase][y + increase] == (enemyColor | Piece::queen) || board[x - increase][y + increase] == (enemyColor | Piece::bishop))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[x - increase][y + increase] != (Piece::king | yourColor))
            {
                break;
            }
        }
        ++increase;
    }
    increase = 1;
    //going up to the left
    if (x - increase >= 0 && y - increase >= 0)
    {
        if (board[x - increase][y - increase] == (enemyColor | Piece::king))
        {
            return true;
        }
    }
    while (x - increase >= 0 && y - increase >= 0)
    {
        if (board[x - increase][y - increase] == 0)
        {
            ++increase;
            continue;
        }//if it's an enemy piece.
        else if (board[x - increase][y - increase] == (enemyColor | Piece::queen) || board[x - increase][y - increase] == (enemyColor | Piece::bishop))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[x - increase][y - increase] != (Piece::king | yourColor))
            {
                break;
            }
        }
        ++increase;
    }
    increase = 1;
    //going up to the right

    if (x + increase < boardXBoxes && y - increase >= 0)
    {
        if (board[x + increase][y - increase] == (enemyColor | Piece::king))
        {
            return true;
        }
    }
    while (x + increase < boardXBoxes && y - increase >= 0)
    {
        if (board[x + increase][y - increase] == 0)
        {
            ++increase;
            continue;
        }//if it's an enemy piece.
        else if (board[x + increase][y - increase] == (enemyColor | Piece::queen) || board[x + increase][y - increase] == (enemyColor | Piece::bishop))
        {
            return true;
        }
        else   //if it's your own piece.
        {
            if (board[x + increase][y - increase] != (Piece::king | yourColor))
            {
                break;
            }
        }
        ++increase;
    }


    //check if there are any knights attacking the square.
    if (currentTurn == 'w')
    {
        for (unsigned i = 0; i < blackLocations.getKnightLocations().size(); i++)
        {
            int xDiff = abs(blackLocations.getKnightLocations()[i].x - box.x);
            int yDiff = abs(blackLocations.getKnightLocations()[i].y - box.y);
            int sumOfLeftAndRight = xDiff + yDiff;
            //that means there's a square thats 2-1 between them.
            if (sumOfLeftAndRight == 3 && xDiff != 0 && yDiff != 0)
            {
                return true;
            }

        }
    }
    else
    {
        for (unsigned i = 0; i < whiteLocations.getKnightLocations().size(); i++)
        {
            int xDiff = abs(whiteLocations.getKnightLocations()[i].x - box.x);
            int yDiff = abs(whiteLocations.getKnightLocations()[i].y - box.y);
            int sumOfLeftAndRight = xDiff + yDiff;
            //that means there's a square thats 2-1 between them.
            if (sumOfLeftAndRight == 3 && xDiff != 0 && yDiff != 0)
            {
                return true;
            }
        }
    }
    return false;
}


///***///

bool Board::inSameRow(Box box1, Box box2)
{
    return box1.y == box2.y;
}

bool Board::inSameCol(Box box1, Box box2)
{
    return box1.x == box2.x;
}

//bottom left to top right
bool Board::inSameDiagonal(Box box1, Box box2)
{
    int increase = 1;
    if (box1.x < box2.x)
    {
        while (box1.x + increase <= box2.x)
        {
            if (box1.x + increase == box2.x && box1.y - increase == box2.y)
            {
                return true;
            }
            ++increase;
        }
        return false;
    }
    else
    {
        while (box2.x + increase <= box1.x)
        {
            if (box2.x + increase == box1.x && box2.y - increase == box1.y)
            {
                return true;
            }
            ++increase;
        }
        return false;
    }


}

//top left to bottom right
bool Board::inSameReverseDiagonal(Box box1, Box box2)
{
    int increase = 1;
    if (box1.x < box2.x)
    {
        while (box1.x + increase <= box2.x)
        {
            if (box1.x + increase == box2.x && box1.y + increase == box2.y)
            {
                return true;
            }
            ++increase;
        }
        return false;
    }
    else
    {
        while (box2.x + increase <= box1.x)
        {
            if (box2.x + increase == box1.x && box2.y + increase == box1.y)
            {
                return true;
            }
            ++increase;
        }
        return false;
    }
}
