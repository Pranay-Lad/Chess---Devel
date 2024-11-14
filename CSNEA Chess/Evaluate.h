#pragma once
#include "Board.h"
#include "Piece.h"

class Evaluate {
private:
	// Piece Evaluation Values
	const int m_Pawn = 100;
	const int m_Knight = 300;
	const int m_Bishop = 350;
	const int m_Rook = 500;
	const int m_Queen = 900;
	const int m_King = 2000;
	const int m_threshold = 2000;
			  
public:
	int Square[64]; // Evaluation Array
	int totalPoints;

	// Methods
	void convertToEvaluate(int Square[64]);
	void PrintBoard();
	float evaluateValue();
	void evaluationBar(sf::RectangleShape& BlackBar);
};