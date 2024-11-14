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

bool Piece::isKingMoveValid(int startRow, int startCol, int endRow, int endCol, bool hasCastled, int Square[64], bool isWhite) {
    if (startRow == endRow && startCol == endCol) {
        return false; // Piece cannot move onto its start square
    }
    int rowDiff = abs(startRow - endRow); // Calculate the difference between 
    int colDiff = abs(startCol - endCol); // rows and columns

    int row = (isWhite) ? 7 : 0;
    int startIndex = Helpers::toIndex(startRow, startCol);
    int endIndex = Helpers::toIndex(endRow, endCol);
    int offset = (startIndex > endIndex) ? -1 : 1;
    // Add Castling Rules
    if (!hasCastled && endRow == row) {
        if ((startIndex + (2* offset) == endIndex)) {
            if (Square[startIndex + offset] == 0 && Square[startIndex + (offset * 2)] == 0) {
                if (offset == -1 && Square[startIndex + (offset * 3)] == 0 || offset == 1) {
                    return true;
                }
                
            }
            
        }
    }


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
        return false; // Pieces cannot move onto their start square
    }
    int direction = isWhite ? -1 : 1; // White pawns move up (negative direction), black pawns move down (positive direction)
    bool isStartRow = (isWhite && startRow == 6) || (!isWhite && startRow == 1); // Check if the pawn is on its initial row
    int colour = (isWhite) ? 0 : 8; // Ternary operator assigns 0 for white and 8 for black
    int offset = Helpers::toIndex(endRow, endCol) - Helpers::toIndex(startRow, startCol); // Calculate offset between start square and target square
    bool isCapture = Piece::isOccupiedHostile(endRow, endCol, colour, Square); // Check if square has an enemy piece


    // Simple forward move (1 square)
    if (endCol == startCol && endRow == startRow + direction && isCapture) {
        return true;
    }

    // Double move forward from starting position (2 squares)
    if (isStartRow && endCol == startCol && endRow == startRow + 2 * direction && isCapture) {
        return true;
    }

    
    if (offset == (direction * -7) || offset == (direction * -9)) { // If the offsets match pawn offsets
        if (!isCapture) { 
            return true; // allow capture
        }
    }
    // Invalid move
    return false;
}

// Return the Sprite at a Grid Position
sf::Sprite& Piece::returnAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset) {
    int targetIndex = Helpers::toIndex(endRow, endCol); // Calculate index
    for (int i = 0; i < Pieces.size(); i++) { // Iterate for size of the vector
        int index = Helpers::getIndex(Pieces[i].getPosition(), squareSize, xOffset);
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

// Validate Castling Move
bool Piece::isCastlingValid(int startRow, int startCol, int endRow, int endCol, bool isWhite, int Square[64]) {
    if (startRow != endRow) { return false; }
    int colour = (isWhite) ? 0 : 8;
    int kingIndex = Helpers::toIndex(startRow, startCol);
    if ((Square[kingIndex] - colour) != 6) { return false; }
    int rookIndex;

}

// Check if Promotion Possible
bool Piece::isPromotion(int startRow, int startCol, int endRow, int endCol, bool isWhite, int Square[64]) {
    int targetIndex = Helpers::toIndex(endRow, endCol); // Calculate Target Index
    std::cout << "END ROW" << endRow  << std::endl;
    int promotionRow = (isWhite) ? 0 : 7; // Set the final rank for white and black
    std::cout << "PROMOTION ROW" << promotionRow << std::endl;
    if (endRow  == (promotionRow)) { // If Final Rank Reached
        return true; 
    }
    return false;
}