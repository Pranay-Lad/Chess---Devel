#pragma once
#include "Board.h"
#include "Piece.h"

class Evaluate {
public:
	const int Pawn = 1;
	const int Knight = 300;
	const int Bishop = 320;
	const int Rook = 500;
	const int Queen = 900;

	int Square[64];

	void convertToEvaluate(int Square[64]);
	void PrintBoard();
	int evaluateValue();
};