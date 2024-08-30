#include "Piece.h"


bool Piece::isPositionValid(int row, int col) {
	return row >= 0 && row < 8 && col >= 0 && col < 8; // Check that Position is in grid
}

bool Piece::isRookMoveValid(int startRow, int startCol, int endRow, int endCol) {
    if (startRow == endRow && startCol == endCol) {
        return false; // Piece cannot move onto its start square
    }
    if (startRow != endRow && startCol != endCol) return false;  // Rook moves in a straight line
    // If move is pseudolegal
    return true;
}


bool Piece::isBishopMoveValid(int startRow, int startCol, int endRow, int endCol) {
    if (startRow == endRow && startCol == endCol) {
        return false; // Piece cannot move onto its start square
    }
    if (abs(startRow - endRow) != abs(startCol - endCol)) return false;  // Bishop moves diagonally
    // If move is pseudolegal
    return true;
}

bool Piece::isQueenMoveValid(int startRow, int startCol, int endRow, int endCol) {
    if (startRow == endRow && startCol == endCol) {
        return false; // Piece cannot move onto its start square
    }
    // Queen's movement is a combination of Rook and Bishop
    return Piece::isRookMoveValid(startRow, startCol, endRow, endCol) ||
        Piece::isBishopMoveValid(startRow, startCol, endRow, endCol);
}

bool Piece::isKingMoveValid(int startRow, int startCol, int endRow, int endCol) {
    if (startRow == endRow && startCol == endCol) {
        return false; // Piece cannot move onto its start square
    }
    int rowDiff = abs(startRow - endRow); // Calculate the difference between 
    int colDiff = abs(startCol - endCol); // rows and columns

    return rowDiff <= 1 && colDiff <= 1;  // King moves one square in any direction
}

bool Piece::isKnightMoveValid(int startRow, int startCol, int endRow, int endCol) {
    if (startRow == endRow && startCol == endCol) {
        return false; // Piece cannot move onto its start square
    }
    int rowDiff = abs(startRow - endRow); // Calculate difference between
    int colDiff = abs(startCol - endCol); // rows and columns

    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);  // "L" shape movement
}

bool Piece::isPawnMoveValid(int startRow, int startCol, int endRow, int endCol, bool isWhite, int Square[64]) {
    if (startRow == endRow && startCol == endCol) {
        return false;
    }
    int direction = isWhite ? -1 : 1; // White pawns move up (negative direction), black pawns move down (positive direction)
    bool isStartRow = (isWhite && startRow == 6) || (!isWhite && startRow == 1); // Check if the pawn is on its initial row
    int colour = (isWhite) ? 0 : 8; // Ternary operator assigns 0 for white and 8 for black


    // Simple forward move (1 square)
    if (endCol == startCol && endRow == startRow + direction) {
        return true;
    }

    // Double move forward from starting position (2 squares)
    if (isStartRow && endCol == startCol && endRow == startRow + 2 * direction) {
        return true;
    }

    int index = Core::toIndex(endRow, endCol) - Core::toIndex(startRow, startCol); // Calculate offset between start square and target square
    bool isCapture = Piece::isOccupiedHostile(endRow, endCol, colour, Square); // Check if square has an enemy piece
    if (index == (direction * -7) || index == (direction * -9)) { // If the offsets match pawn offsets
        if (!isCapture) { 
            return true; // allow capture
        }
    }
    // Invalid move
    return false;
}

// Return the Sprite at a Grid Position
sf::Sprite Piece::returnAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset) {
    int targetIndex = Core::toIndex(endRow, endCol); // Calculate index
    for (int i = 0; i < Pieces.size(); i++) { // Iterate for size of the vector
        int index = Core::getIndex(Pieces[i].getPosition(), squareSize, xOffset);
        if (index == targetIndex) { // If Target index = Sprite Index
            return Pieces[i]; // Return the Sprite
        }
    }
}

bool Piece::isOccupiedHostile(int row, int col, int Colour, int Square[64]) {
    int index = ((7 - row) * 8) + col;
    if (Square[index] == 0) {
        return true;
    }
    if (Colour == 8) {
        if (Square[index] < 8) {
            return false;
        }
        return true;
    }
    if (Square[index] > 8) {
        return false;
    }
    return true;
}