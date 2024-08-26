#pragma once

#include <array>
#include <vector>
#include <string>
#include "Piece.h"
#include <iostream>
#include <unordered_map>

namespace Core {
	class Board
	{
	private:
	public: 
		int Square[64];
		
	public:
		Board();

		void InitialiseBoard();
		void PrintBoard();

		bool hasWhiteCastled;
		bool hasBlackCastled;

		bool isOccupiedFriendly(int row, int col, int Colour);
	};

}

