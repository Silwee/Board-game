#include "Chess_StoreMove.h"


void StoreMove::init(StoreMove* nextStore, Move newMove, bool updatedKing, bool updatedQueen, Box previousPassant, bool took, uint8_t piece)
{
    next = nextStore;
    move =
    {
        newMove.fromBox,
        newMove.toBox,
        newMove.kingSideCastle,
        newMove.queenSideCastle,
        newMove.enPassant,
        newMove.isPromotion,
        newMove.promotionType
    };

    previousWhiteKingSide = previousBlackKingSide = previousWhiteQueenSide = previousBlackQueenSide = false;
    previousEnPassant = previousPassant;
    tookAPiece = took;
    pieceTaken = piece;
}

StoreMove::StoreMove()
{

}

void StoreMove::setNext(StoreMove* newNext)
{
    next = newNext;
}

StoreMove* StoreMove::getNext()
{
    return next;
}


Move StoreMove::getMove()
{
    return move;
}

bool StoreMove::tookPiece()
{
    return tookAPiece;
}

Box StoreMove::getPreviousEnPassant()
{
    return previousEnPassant;
}
bool StoreMove::getPreviousWhiteKingSide()
{
    return previousWhiteKingSide;
}
bool StoreMove::getPreviousBlackKingSide()
{
    return previousBlackKingSide;
}
bool StoreMove::getPreviousWhiteQueenSide()
{
    return previousWhiteQueenSide;
}
bool StoreMove::getPreviousBlackQueenSide()
{
    return previousBlackQueenSide;
}

bool StoreMove::getPawnMove()
{
    return isPawnMove;
}
uint8_t StoreMove::getTakenPiece()
{
    return pieceTaken;
}
int StoreMove::getPreviousHalfMoveClock()
{
    return previousHalfMoveClock;
}

void StoreMove::setMove(Move newMove)
{
    move =
    {
        newMove.fromBox,
        newMove.toBox,
        newMove.kingSideCastle,
        newMove.queenSideCastle,
        newMove.enPassant,
        newMove.isPromotion,
        newMove.promotionType
    };

}

void StoreMove::setPreviousCastles(bool whiteKing, bool whiteQueen, bool blackKing, bool blackQueen)
{
    previousWhiteKingSide = whiteKing;
    previousWhiteQueenSide = whiteQueen;
    previousBlackKingSide = blackKing;
    previousBlackQueenSide = blackQueen;
}
void StoreMove::setPreviousEnPassant(Box newBox)
{
    previousEnPassant = newBox;
}

void StoreMove::setTookPiece(bool newSet)
{
    tookAPiece = newSet;
}
void StoreMove::setPieceTaken(uint8_t newPiece)
{
    pieceTaken = newPiece;
}
void StoreMove::setPawnMove(bool pawnMove)
{
    isPawnMove = pawnMove;
}
void StoreMove::setPreviousHalfMoveClock(int halfMoveClock)
{
    previousHalfMoveClock = halfMoveClock;
}

void StoreMove::setThreatInfo(uint8_t whiteThreatened, uint8_t blackThreatened, uint8_t whiteAttacked, uint8_t blackAttacked, int amountAttacked, bool attackedByKnight,Box attackedFromBox)
{
    previousWhiteThreatenedInfo = whiteThreatened;
    previousBlackThreatenedInfo = blackThreatened;
    previousWhiteAttackedInfo = whiteAttacked;
    previousBlackAttackedInfo = blackAttacked;
    previousAmountAttacked = amountAttacked;
    previousAttackedByKnight = attackedByKnight;
    previousAttackedFromBox = attackedFromBox;

}
bool StoreMove::getPreviousAttackedByKnight()
{
    return previousAttackedByKnight;
}
uint8_t StoreMove::getPreviousWhiteThreatenedInfo()
{
    return previousWhiteThreatenedInfo;
}
uint8_t StoreMove::getPreviousBlackThreatenedInfo()
{
    return previousBlackThreatenedInfo;
}
uint8_t StoreMove::getPreviousWhiteAttackedInfo()
{
    return previousWhiteAttackedInfo;
}

uint8_t StoreMove::getPreviousBlackAttackedInfo()
{
    return previousBlackAttackedInfo;
}

int StoreMove::getPreviousAmountAttacked()
{
    return previousAmountAttacked;
}
Box StoreMove::getPreviousAttackedFromBox()
{
    return previousAttackedFromBox;
}

void StoreMove::setThreatBoxes(Box whiteStraightLeftBox, Box whiteUpLeftBox, Box whiteStraightUpBox,
                               Box whiteUpRightBox, Box whiteStraightRightBox, Box whiteDownRightBox,
                               Box whiteStraightDownBox, Box whiteDownLeftBox,
                               Box blackStraightLeftBox, Box blackUpLeftBox, Box blackStraightUpBox,
                               Box blackUpRightBox, Box blackStraightRightBox, Box blackDownRightBox,
                               Box blackStraightDownBox, Box blackDownLeftBox)
{
    previousWhiteStraightLeftBox = whiteStraightLeftBox;
    previousWhiteStraightRightBox = whiteStraightRightBox;
    previousWhiteStraightUpBox = whiteStraightUpBox;
    previousWhiteStraightDownBox = whiteStraightDownBox;
    previousWhiteUpLeftBox = whiteUpLeftBox;
    previousWhiteUpRightBox = whiteUpRightBox;
    previousWhiteDownLeftBox = whiteDownLeftBox;
    previousWhiteDownRightBox = whiteDownRightBox;

    previousBlackStraightLeftBox = blackStraightLeftBox;
    previousBlackStraightRightBox = blackStraightRightBox;
    previousBlackStraightUpBox = blackStraightUpBox;
    previousBlackStraightDownBox = blackStraightDownBox;
    previousBlackUpLeftBox = blackUpLeftBox;
    previousBlackUpRightBox = blackUpRightBox;
    previousBlackDownLeftBox = blackDownLeftBox;
    previousBlackDownRightBox = blackDownRightBox;
}

Box StoreMove::getPreviousWhiteStraightLeftBox()
{
    return previousWhiteStraightLeftBox;
}
Box StoreMove::getPreviousWhiteStraightDownBox()
{
    return previousWhiteStraightDownBox;
}

Box StoreMove::getPreviousWhiteStraightRightBox()
{
    return previousWhiteStraightRightBox;
}

Box StoreMove::getPreviousWhiteStraightUpBox()
{
    return previousWhiteStraightUpBox;
}

Box StoreMove::getPreviousWhiteUpLeftBox()
{
    return previousWhiteUpLeftBox;
}

Box StoreMove::getPreviousWhiteDownLeftBox()
{
    return previousWhiteDownLeftBox;
}

Box StoreMove::getPreviousWhiteDownRightBox()
{
    return previousWhiteDownRightBox;
}

Box StoreMove::getPreviousWhiteUpRightBox()
{
    return previousWhiteUpRightBox;
}



Box StoreMove::getPreviousBlackStraightLeftBox()
{
    return previousBlackStraightLeftBox;
}
Box StoreMove::getPreviousBlackStraightDownBox()
{
    return previousBlackStraightDownBox;
}

Box StoreMove::getPreviousBlackStraightRightBox()
{
    return previousBlackStraightRightBox;
}

Box StoreMove::getPreviousBlackStraightUpBox()
{
    return previousBlackStraightUpBox;
}

Box StoreMove::getPreviousBlackUpLeftBox()
{
    return previousBlackUpLeftBox;
}

Box StoreMove::getPreviousBlackDownLeftBox()
{
    return previousBlackDownLeftBox;
}

Box StoreMove::getPreviousBlackDownRightBox()
{
    return previousBlackDownRightBox;
}

Box StoreMove::getPreviousBlackUpRightBox()
{
    return previousBlackUpRightBox;
}
