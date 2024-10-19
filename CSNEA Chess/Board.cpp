#include "Board.h"

// Class Constructor
Core::Board::Board() {
    hasWhiteCastled = false; // Initialise Board Castling Values
    hasBlackCastled = false;
}

// Setup Internal Board For GameStart
void Core::Board::InitialiseBoard() {

    Square[0] = Piece::Rook | Piece::White;
    Square[1] = Piece::Knight | Piece::White;
    Square[2] = Piece::Bishop | Piece::White;
    Square[3] = Piece::Queen | Piece::White;
    Square[4] = Piece::King | Piece::White;
    Square[5] = Piece::Bishop | Piece::White;
    Square[6] = Piece::Knight | Piece::White;
    Square[7] = Piece::Rook | Piece::White;
    
    for (int x = 8; x < 16; x++) {
        Square[x] = Piece::Pawn | Piece::White;
    }

    for (int x = 16; x < 48; x++) {
        Square[x] = Piece::None;
    }

    for (int x = 48; x < 56; x++) {
        Square[x] = Piece::Pawn | Piece::Black;
    }


    Square[56] = Piece::Rook | Piece::Black;
    Square[57] = Piece::Knight | Piece::Black;
    Square[58] = Piece::Bishop | Piece::Black;
    Square[59] = Piece::Queen | Piece::Black;
    Square[60] = Piece::King | Piece::Black;
    Square[61] = Piece::Bishop | Piece::Black;
    Square[62] = Piece::Knight | Piece::Black;
    Square[63] = Piece::Rook | Piece::Black;

}

// Print Board To Console -> Useful for debugging
void Core::Board::PrintBoard() {
    // Loop through each row
    for (int row = 7; row >= 0; --row) {
        // Loop through each column in the current row
        for (int col = 0; col < 8; ++col) {
            // Calculate the index in the 1D array
            int index = row * 8 + col;
            // Print the value at the current index followed by a space
            std::cout << Square[index] << " ";
        }
        // Print a new line at the end of each row
        std::cout << std::endl;
    }
}

bool Core::Board::isOccupiedFriendly(int row, int col, int Colour) {
    int index = ((7 - row) * 8) + col; // Calculate index of Piece
    if (Colour == 8) { // Check if piece is black
        if (Square[index] > 8) {
            return false; // Target square is friendly
        }
        return true; // Piece is Enemy or 0
    }
    // Piece is white
    if (Square[index] > 8 || Square[index] == 0) {
        return true; // Piece is Enemy or 0
    }
    return false; // Target square is Friendly
}

void Core::Board::MakeMove(Move move) {
    Square[move.TargetSquare] = Square[move.StartSquare]; // Move Piece from start square to target square
    Square[move.StartSquare] = NULL; // Set the start square to 0
}

// Check If the Path of a Piece is Blocked by Another Piece
bool Core::Board::isBlocked(int startSquare, int endSquare) {
    // Determine direction of movement
    int dx = (endSquare % 8) - (startSquare % 8); // Column difference
    int dy = (endSquare / 8) - (startSquare / 8); // Row difference

    // Normalize direction vectors
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    // Move in the direction and check for blocks
    int currentSquare = startSquare + stepX + stepY * 8; // Initial step
    while (currentSquare != endSquare) {
        if (currentSquare < 0 || currentSquare >= 64) {
            // Out of bounds
            return true;
        }
        if (Square[currentSquare] != 0) {
            // There is a piece blocking the path
            return true;
        }
        currentSquare += stepX + stepY * 8;
    }

    // No pieces blocking the path
    return false;
}



// Remove Sprite At Grid Position
void Core::Board::removeAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset) {
    int targetindex = Helpers::toIndex(endRow, endCol); // Calculate Index of Target Square
    for (int i = 0; i < Pieces.size(); i++) { // Iterate for the size of the Piece vector
        int index = Helpers::getIndex(Pieces[i].getPosition(), squareSize, xOffset); // Calculate index of sprite
        if (index == targetindex) { // If Position are the same
            Pieces.erase(Pieces.begin() + i); // Remove Sprite from vector
        }

    }
}

sf::Sprite& Core::Board::getAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset) {
    int targetindex = Helpers::toIndex(endRow, endCol); // Calculate Index of Target Square
    for (int i = 0; i < Pieces.size(); i++) { // Iterate for the size of the Piece vector
        int index = Helpers::getIndex(Pieces[i].getPosition(), squareSize, xOffset); // Calculate index of sprite
        if (index == targetindex) { // If Position are the same
            return Pieces[i]; // Remove Sprite from vector
        }
    }
}

void Core::Board::moveAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset, int targetRow, int targetCol) {  
    int index = (endRow * 8) + endCol;
    if (Square[index] == (Piece::Black | Piece::Pawn)) {
        endRow = 7 - endRow;
        targetRow = 7 - targetRow;
        sf::Sprite& piece = Core::Board::getAtPosition(endRow, endCol, Pieces, squareSize, xOffset);
        sf::Vector2f pos = Helpers::getSnappedPosition(piece.getPosition(), squareSize, xOffset);
        std::cout << "X: " << pos.x << "Y: " << pos.y << std::endl;
        sf::Vector2f snappedPosition = Helpers::getSnappedPosition(targetRow, targetCol, squareSize, xOffset);
        piece.setPosition(Helpers::getCenteredPosition(snappedPosition, piece, squareSize));
        Core::Board::MakeMove(Move(Helpers::toIndex(endRow, endCol), Helpers::toIndex(targetRow, targetCol)));
    }
    
}

bool Core::Board::isKingInCheck(std::vector<sf::Sprite> King, bool isWhite, int squareSize, int xOffset) {
    int kingIndex = Helpers::getIndex(King[0].getPosition(), squareSize, xOffset);
    std::cout << "KING INDEX" << kingIndex << std::endl;
    // Piece values for the opponent based on the color of the king
    int opponentPawn = isWhite ? 9 : 1;
    int opponentKnight = isWhite ? 10 : 2;
    int opponentBishop = isWhite ? 11 : 3;
    int opponentRook = isWhite ? 12 : 4;
    int opponentQueen = isWhite ? 13 : 5;
    int opponentKing = isWhite ? 14 : 6;

    // Check for opponent's pawns attacking the king
    int pawnDirection = isWhite ? 1 : -1;
    int pawnAttacks[2] = { kingIndex + 7 * pawnDirection, kingIndex + 9 * pawnDirection };  // Diagonal attack squares
    for (int attackIndex : pawnAttacks) {
        if (attackIndex >= 0 && attackIndex < 64 && Square[attackIndex] == opponentPawn) {
            return true;
        }
    }

    // Check for opponent's knights attacking the king
    int knightMoves[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
    for (int move : knightMoves) {
        int targetIndex = kingIndex + move;
        if (targetIndex >= 0 && targetIndex < 64 && Square[targetIndex] == opponentKnight) {
            return true;
        }
    }

    // Check for opponent's bishops and queens (diagonal moves) attacking the king
    int bishopDirections[4] = { -9, -7, 7, 9 };
    for (int direction : bishopDirections) {
        for (int i = 1; i < 8; i++) {
            int targetIndex = kingIndex + i * direction;
            if (targetIndex < 0 || targetIndex >= 64 || (targetIndex % 8 == 0 && direction == 9) || ((targetIndex + 1) % 8 == 0 && direction == 7)) {
                break;  // Out of bounds or wrapping around the board
            }
            if (Square[targetIndex] != 0) {
                if (Square[targetIndex] == opponentBishop || Square[targetIndex] == opponentQueen) {
                    return true;
                }
                break;  // Blocked by another piece
            }
        }
    }

    // Check for opponent's rooks and queens (straight moves) attacking the king
    int rookDirections[4] = { -8, -1, 1, 8 };
    for (int direction : rookDirections) {
        for (int i = 1; i < 8; i++) {
            int targetIndex = kingIndex + i * direction;
            if (targetIndex < 0 || targetIndex >= 64 || (targetIndex % 8 == 0 && direction == 1) || ((targetIndex + 1) % 8 == 0 && direction == -1)) {
                break;  // Out of bounds or wrapping around the board
            }
            if (Square[targetIndex] != 0) {
                if (Square[targetIndex] == opponentRook || Square[targetIndex] == opponentQueen) {
                    return true;
                }
                break;  // Blocked by another piece
            }
        }
    }

    // Check for opponent's king attacking the king (1 square around the king)
    int kingMoves[8] = { -9, -8, -7, -1, 1, 7, 8, 9 };
    for (int move : kingMoves) {
        int targetIndex = kingIndex + move;
        if (targetIndex >= 0 && targetIndex < 64 && Square[targetIndex] == opponentKing) {
            return true;
        }
    }

    // No threats found, king is not in check
    return false;
}

void Core::Board::addAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset) {
    // Add A Sprite To Vector At Specified Position and Scale
    sf::Sprite Piece;
    Piece.scale(0.7f, 0.7f);
    Piece.setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(endRow, endCol, squareSize, xOffset), Piece, squareSize));
    Pieces.push_back(Piece);
}