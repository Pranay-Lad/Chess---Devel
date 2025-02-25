#include "Evaluate.h"

// Convert the board to an evaluation board
void Evaluate::convertToEvaluate(int BoardSquare[64]) {
	// Loop across the board
	for (int i = 0; i < 64; i++) {
		int value = (BoardSquare[i] >= 8) ? (BoardSquare[i] - Piece::Black) : BoardSquare[i];
		int colour = (BoardSquare[i] >= 8) ? -1 : 1;
		// Offset depending on piece colour
		switch (value) {
		case 1:
			// Set Pawn value 100
			Square[i] = m_Pawn * colour;
			break;
		case 2:
			// Set Knight Value 300
			Square[i] = m_Knight * colour;
			break;
		case 3:
			// Set Bishop Value 350
			Square[i] = m_Bishop * colour;
			break;
		case 4:
			// Set Rook Value 350
			Square[i] = m_Rook * colour;
			break;
		case 5:
			// Set Queen Value 500
			Square[i] = m_Queen * colour;
			break;
		case 6:
			// Set King Value 2000
			Square[i] = m_King * colour;
			break;
		default:
			// Set Empty Square 0
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

// Calculate Current Evaluation
float Evaluate::evaluateValue() {
	int value = 0;
	// Loop Through Board
	for (auto& piece : Square) {
		value += piece;
		// Sum evaluation
	}
	return value;
}

// Update the Evaluation Bar
void Evaluate::evaluationBar(sf::RectangleShape& BlackBar) {
	float evaluation = evaluateValue(); // Calculate Evaluation
	float value = 0;
	// If Threshold Reached, Bar Full For Correct Player
	if (evaluation > m_threshold) {
		value = 0.0f;
	}
	else if (evaluation < (-1 * m_threshold)) {
		value = 800.0f;
	}
	else {
		// Add Offset From Equal Evaluation
		value =  400.0f + ((evaluation / m_threshold)) * 800.0f;
	}
	if (evaluation == 0) {
		// If Equal Position, Set Equal Bar Length
		BlackBar.setSize(sf::Vector2f(40.0f, 400.0f));
	}
	else {
		// Set Bar Proportional To |Evaluation| / Threshold
		BlackBar.setSize(sf::Vector2f(40.0f, (800-value)));
	}
	std::cout << "EVALUATION" << evaluation << std::endl;
	
}
