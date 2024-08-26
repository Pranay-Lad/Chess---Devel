#pragma once

#include <array>
#include <vector>
#include <string>
#include "Piece.h"
#include <iostream>
#include <unordered_map>

#include "Move.h"

namespace Core {
	class Board
	{
	private:
	public: 
		int Square[64];
		bool hasWhiteCastled;
		bool hasBlackCastled;
		
	public:
		Board();

		void InitialiseBoard();
		void PrintBoard();
		void MakeMove(Move move);

		bool isOccupiedFriendly(int row, int col, int Colour);
		bool isBlocked(int startSquare, int endSquare);
	};

}

