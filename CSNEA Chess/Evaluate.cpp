#include "Evaluate.h"

void Evaluate::convertToEvaluate(int BoardSquare[64]) {
	for (int i = 0; i < 64; i++) {
		int value = (BoardSquare[i] >= 8) ? (BoardSquare[i] - Piece::Black) : BoardSquare[i];
		int colour = (BoardSquare[i] >= 8) ? -1 : 1;
		//std::cout << "VALUE: " << value << std::endl;
		switch (value) {
		case 1:
			Square[i] = 100 * colour;
			break;
		case 2:
			Square[i] = 300 * colour;
			break;
		case 3:
			Square[i] = 320 * colour;
			break;
		case 4:
			Square[i] = 500 * colour;
			break;
		case 5:
			Square[i] = 900 * colour;
			break;
		case 6:
			Square[i] = 2000 * colour;
			break;
		default:
			Square[i] = 0;
		}
	}
}

void Evaluate::PrintBoard() {
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

int Evaluate::evaluateValue() {
	int value = 0;
	for (auto& piece : Square) {
		value += piece;
	}
	return value;
}