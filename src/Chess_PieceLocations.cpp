#include "Chess_PieceLocations.h"
#include "Chess_Piece.h"

Box& PieceLocations::getKingLocation()
{
    return kingLocation;
}

std::vector<Box>& PieceLocations::getPawnLocations()
{
    return pawnLocations;
}

std::vector<Box>& PieceLocations::getKnightLocations()
{
    return knightLocations;
}

std::vector<Box>& PieceLocations::getRookLocations()
{
    return rookLocations;
}

std::vector<Box>& PieceLocations::getBishopLocations()
{
    return bishopLocations;
}

std::vector<Box>& PieceLocations::getQueenLocations()
{
    return queenLocations;
}

int PieceLocations::numOfBishop()
{
    return bishopLocations.size();
}

int PieceLocations::numOfKnight()
{
    return knightLocations.size();
}

int PieceLocations::numOfRook()
{
    return rookLocations.size();
}

int PieceLocations::numOfQueen()
{
    return queenLocations.size();
}

int PieceLocations::numOfPawn()
{
    return pawnLocations.size();
}

void PieceLocations::removePawn(Box removeBox)
{
    for (int i = 0; i < pawnLocations.size(); i++)
    {
        if (pawnLocations[i] == removeBox)
        {
            pawnLocations.erase(pawnLocations.begin() + i);
            return;
        }
    }
}

void PieceLocations::removeKnight(Box removeBox)
{
    for (int i = 0; i < knightLocations.size(); i++)
    {
        if (knightLocations[i] == removeBox)
        {
            knightLocations.erase(knightLocations.begin() + i);
            return;
        }
    }
}

void PieceLocations::removeRook(Box removeBox)
{
    for (int i = 0; i < rookLocations.size(); i++)
    {
        if (rookLocations[i] == removeBox)
        {
            rookLocations.erase(rookLocations.begin() + i);
            return;
        }
    }
}

void PieceLocations::removeBishop(Box removeBox)
{
    for (int i = 0; i < bishopLocations.size(); i++)
    {
        if (bishopLocations[i] == removeBox)
        {
            bishopLocations.erase(bishopLocations.begin() + i);
            return;
        }
    }
}

void PieceLocations::removeQueen(Box removeBox)
{
    for (int i = 0; i < queenLocations.size(); i++)
    {
        if (queenLocations[i] == removeBox)
        {
            queenLocations.erase(queenLocations.begin() + i);
            return;
        }
    }
}


void PieceLocations::updatePawn(Box oldBox, Box newBox)
{
    for (int i = 0; i < pawnLocations.size(); i++)
    {
        if (pawnLocations[i] == oldBox)
        {
            pawnLocations[i] = newBox;
            return;
        }
    }
}

void PieceLocations::updateQueen(Box oldBox, Box newBox)
{
    for (int i = 0; i < queenLocations.size(); i++)
    {
        if (queenLocations[i] == oldBox)
        {
            queenLocations[i] = newBox;
            return;
        }
    }
}

void PieceLocations::updateBishop(Box oldBox, Box newBox)
{
    for (int i = 0; i < bishopLocations.size(); i++)
    {
        if (bishopLocations[i] == oldBox)
        {
            bishopLocations[i] = newBox;
            return;
        }
    }
}

void PieceLocations::updateRook(Box oldBox, Box newBox)
{
    for (int i = 0; i < rookLocations.size(); i++)
    {
        if (rookLocations[i] == oldBox)
        {
            rookLocations[i] = newBox;
            return;
        }
    }
}

void PieceLocations::updateKnight(Box oldBox, Box newBox)
{
    for (int i = 0; i < knightLocations.size(); i++)
    {
        if (knightLocations[i] == oldBox)
        {
            knightLocations[i] = newBox;
            return;
        }
    }
}


void PieceLocations::removePiece(uint8_t piece, Box box)
{
    if ((piece & Piece::knight) == Piece::knight)
    {
        removeKnight(box);
    }
    else if ((piece & Piece::pawn) == Piece::pawn)
    {
        removePawn(box);
    }
    else if ((piece & Piece::queen) == Piece::queen)
    {
        removeQueen(box);
    }
    else if ((piece & Piece::bishop) == Piece::bishop)
    {
        removeBishop(box);
    }
    else if ((piece & Piece::rook) == Piece::rook)
    {
        removeRook(box);
    }
}

void PieceLocations::updatePiece(uint8_t piece, Box fromBox, Box toBox)
{
    if ((piece & Piece::knight) == Piece::knight)
    {
        updateKnight(fromBox, toBox);
    }
    else if ((piece & Piece::pawn) == Piece::pawn)
    {
        updatePawn(fromBox, toBox);
    }
    else if ((piece & Piece::queen) == Piece::queen)
    {
        updateQueen(fromBox, toBox);
    }
    else if ((piece & Piece::bishop) == Piece::bishop)
    {
        updateBishop(fromBox, toBox);
    }
    else if ((piece & Piece::rook) == Piece::rook)
    {
        updateRook(fromBox, toBox);
    }
    else if ((piece & Piece::king) == Piece::king)
    {
        setKingLocation(toBox);
    }
}

void PieceLocations::addPiece(uint8_t piece, Box newBox)
{
    if ((piece & Piece::knight) == Piece::knight)
    {
        knightLocations.push_back(newBox);
    }
    else if ((piece & Piece::pawn) == Piece::pawn)
    {
        pawnLocations.push_back(newBox);
    }
    else if ((piece & Piece::queen) == Piece::queen)
    {
        queenLocations.push_back(newBox);
    }
    else if ((piece & Piece::bishop) == Piece::bishop)
    {
        bishopLocations.push_back(newBox);
    }
    else if ((piece & Piece::rook) == Piece::rook)
    {
        rookLocations.push_back(newBox);
    }
    else if ((piece & Piece::king) == Piece::king)
    {
        setKingLocation(newBox);
    }
}

void PieceLocations::setKingLocation(Box newLoc)
{
    kingLocation = newLoc;
}

void PieceLocations::clear()
{
    pawnLocations.clear();
    knightLocations.clear();
    rookLocations.clear();
    bishopLocations.clear();
    queenLocations.clear();
}
