#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>

#include "Helpers.h"
#include "Board.h"


namespace Piece {
    const int None = 0; // E.g. Piece::Black | Piece:: Knight = 10
    const int Pawn = 1;
    const int Knight = 2;
    const int Bishop = 3;
    const int Rook = 4;
    const int Queen = 5;
    const int King = 6;
    const int White = 0;
    const int Black = 8;

    // Move Validation Functions
    bool isPositionValid(int row, int col);
    bool isRookMoveValid(int startRow, int startCol, int endRow, int endCol);
    bool isBishopMoveValid(int startRow, int startCol, int endRow, int endCol );
    bool isQueenMoveValid(int startRow, int startCol, int endRow, int endCol);
    bool isKingMoveValid(int startRow, int startCol, int endRow, int endCol, bool hasCastled, int Square[64], bool isWhite);
    bool isKnightMoveValid(int startRow, int startCol, int endRow, int endCol);
    bool isPawnMoveValid(int startRow, int startCol, int endRow, int endCol, bool isWhite, int Square[64]);
    bool isCastlingValid(int startRow, int startCol, int endRow, int endCol, bool isWhite, int Square[64]);

    bool isPromotion(int startRow, int startCol, int endRow, int endCol, bool isWhite, int Square[64]);

    // Check if Square is Hostile
    bool isOccupiedHostile(int row, int col, int Colour, int Square[64]);

    // Return Reference to Sprite
    sf::Sprite& returnAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset);
}