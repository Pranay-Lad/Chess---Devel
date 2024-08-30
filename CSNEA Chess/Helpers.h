#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>


namespace Core {
	void printSquareLabel(sf::Vector2f position, int squareSize, int xOffset, int PieceName);

	sf::Vector2f getSnappedPosition(sf::Vector2f position, int squareSize, int xOffset, const sf::Sprite& piece);
	sf::Vector2f getSnappedPosition(sf::Vector2f position, int squareSize, int xOffset);
	sf::Vector2f getSnappedPosition(int row, int col, int squareSize, int xOffset);

	sf::Vector2f getCenteredPosition(sf::Vector2f squarePosition, const sf::Sprite& piece, int squareSize);

	void SetupPawns(std::vector<sf::Sprite>& Pawns, sf::Texture& Texture, int xOffset, int squareSize, int rank);
	void SetupPieces(std::vector<sf::Sprite>& Pieces, sf::Texture& Texture, int xOffset, int squareSize, int Rank, int Pos);
	void SetupPieces(std::vector<sf::Sprite>& Pieces, sf::Texture& Texture, int xOffset, int squareSize, int Rank, int Pos_1, int Pos_2);

	int getIndex(sf::Vector2f position, int squareSize, int xOffset);
	int toIndex(int row, int col);
}
