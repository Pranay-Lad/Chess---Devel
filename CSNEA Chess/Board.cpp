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
    Square[59] = Piece::King | Piece::Black;
    Square[60] = Piece::Queen | Piece::Black;
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