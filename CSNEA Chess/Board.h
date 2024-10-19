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
		void removeAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset);
		void moveAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset, int targetRow, int TargetCol);
		void addAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset);

		bool isOccupiedFriendly(int row, int col, int Colour);
		bool isBlocked(int startSquare, int endSquare);
		bool isKingInCheck(std::vector<sf::Sprite> King, bool isWhite, int squareSize, int xOffset);
		
		sf::Sprite& getAtPosition(int endRow, int endCol, std::vector<sf::Sprite>& Pieces, int squareSize, int xOffset);
	};

}

