#include "Helpers.h"

void Helpers::printSquareLabel(sf::Vector2f position, int squareSize, int xOffset, int PieceName) {
    int col = static_cast<int>((position.x - xOffset) / squareSize);
    int row = 7 - static_cast<int>(position.y / squareSize);
    std::cout << PieceName << " dropped on square : " << col << "\t" << row << std::endl;
}

sf::Vector2f Helpers::getSnappedPosition(sf::Vector2f position, int squareSize, int xOffset, const sf::Sprite& piece) {
    int col = static_cast<int>((position.x - xOffset) / squareSize);
    int row = static_cast<int>(position.y / squareSize);

    // Calculate the center of the square
    float squareCenterX = xOffset + col * squareSize + squareSize / 2;
    float squareCenterY = row * squareSize + squareSize / 2;

    // Adjust position to center the sprite on the square
    float spriteX = squareCenterX - piece.getGlobalBounds().width / 2;
    float spriteY = squareCenterY - piece.getGlobalBounds().height / 2;

    return sf::Vector2f(spriteX, spriteY);
}

sf::Vector2f Helpers::getSnappedPosition(sf::Vector2f position, int squareSize, int xOffset) {
    // Calculate the column and row based on the top-left corner of the piece
    int col = static_cast<int>((position.x - xOffset) / squareSize);
    int row = static_cast<int>(position.y / squareSize);

    // Calculate the top-left corner of the square to snap the piece to
    float snappedX = xOffset + col * squareSize;
    float snappedY = row * squareSize;

    return sf::Vector2f(snappedX, snappedY);
}

sf::Vector2f Helpers::getSnappedPosition(int row, int col, int squareSize, int xOffset) {
    // Calculate the top-left corner of the square
    float snappedX = xOffset + col * squareSize;
    float snappedY = row * squareSize;

    return sf::Vector2f(snappedX, snappedY);
}

sf::Vector2f Helpers::getCenteredPosition(sf::Vector2f squarePosition, const sf::Sprite& piece, int squareSize) {
    // Calculate the center position based on the top-left corner of the snapped square
    float centeredX = squarePosition.x + (squareSize / 2) - (piece.getGlobalBounds().width / 2);
    float centeredY = squarePosition.y + (squareSize / 2) - (piece.getGlobalBounds().height / 2);

    return sf::Vector2f(centeredX, centeredY);
}

sf::Vector2f Helpers::getCenteredPosition(sf::Vector2f squarePosition, const sf::RectangleShape& shape, int squareSize) {
    float centeredX = squarePosition.x + (squareSize / 2) - (shape.getGlobalBounds().width / 2);
    float centeredY = squarePosition.y + (squareSize / 2) - (shape.getGlobalBounds().height / 2);

    return sf::Vector2f(centeredX, centeredY);
}

// Set Properties For Pawn Sprites
void Helpers::SetupPawns(std::vector<sf::Sprite>& Pawns, sf::Texture& Texture, int xOffset, int squareSize, int Rank) {
    // Loop Across all 8 columns
    for (int col = 0; col < 8; ++col) {
        Pawns[col].setScale(0.7f, 0.7f); // Change size of the piece to fit the board
        Pawns[col].setTexture(Texture);
        sf::Vector2f snappedPosition = Helpers::getSnappedPosition(Rank, col, squareSize, xOffset); // Set Pawn To Grid Position
        Pawns[col].setPosition(Helpers::getCenteredPosition(snappedPosition, Pawns[col], squareSize)); // Adjust position of the piece to the centre of the square
    }
}
// Set Properties For King and Queen
void Helpers::SetupPieces(std::vector<sf::Sprite>& Pieces, sf::Texture& Texture, int xOffset, int squareSize, int Rank, int Pos)
{
    Pieces[0].setScale(0.7f, 0.7f);
    Pieces[0].setTexture(Texture);
    sf::Vector2f snappedPosition = Helpers::getSnappedPosition(Rank, Pos, squareSize, xOffset);
    Pieces[0].setPosition(Helpers::getCenteredPosition(snappedPosition, Pieces[0], squareSize));
}

// Set Properties For Rook, Bishop and Knight
void Helpers::SetupPieces(std::vector<sf::Sprite>& Pieces, sf::Texture& Texture, int xOffset, int squareSize, int Rank, int Pos_1, int Pos_2)
{
    Pieces[0].setScale(0.7f, 0.7f);
    Pieces[0].setTexture(Texture);
    sf::Vector2f snappedPosition = Helpers::getSnappedPosition(Rank, Pos_1, squareSize, xOffset);
    Pieces[0].setPosition(Helpers::getCenteredPosition(snappedPosition, Pieces[0], squareSize));
    Pieces[1].setScale(0.7f, 0.7f);
    Pieces[1].setTexture(Texture);
    snappedPosition = Helpers::getSnappedPosition(Rank, Pos_2, squareSize, xOffset);
    Pieces[1].setPosition(Helpers::getCenteredPosition(snappedPosition, Pieces[1], squareSize));
}

// Returns the Square Index of a Sprite Piece
int Helpers::getIndex(sf::Vector2f position, int squareSize, int xOffset) {
    int col = static_cast<int>((position.x - xOffset) / squareSize);
    int row = 7 - static_cast<int>(position.y / squareSize); // Convert position vector to column and rows
    return ((row * 8) + col); // Return index
}

// Retuns the Square Index of a Piece
int Helpers::toIndex(int row, int col) {
    return (7 - row) * 8 + col;
}
