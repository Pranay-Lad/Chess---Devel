#include <SFML/Graphics.hpp>
#include "Helpers.h"

int main()
{
    // Create window instance
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Chess");
    sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;

    // Import Textures For Pieces
    t1.loadFromFile("Assets/WhitePawn.png");
    t2.loadFromFile("Assets/WhiteKnight.png");
    t3.loadFromFile("Assets/WhiteBishop.png");
    t4.loadFromFile("Assets/WhiteRook.png");
    t5.loadFromFile("Assets/WhiteQueen.png");
    t6.loadFromFile("Assets/WhiteKing.png");
    t7.loadFromFile("Assets/BlackPawn.png");
    t8.loadFromFile("Assets/BlackKnight.png");
    t9.loadFromFile("Assets/BlackBishop.png");
    t10.loadFromFile("Assets/BlackRook.png");
    t11.loadFromFile("Assets/BlackQueen.png");
    t12.loadFromFile("Assets/BlackKing.png");

    // Create Vectors For Each Piece
    std::vector<sf::Sprite> WhitePawns(8);
    std::vector<sf::Sprite> WhiteKnights(2);
    std::vector<sf::Sprite> WhiteBishops(2);
    std::vector<sf::Sprite> WhiteRooks(2);
    std::vector<sf::Sprite> WhiteQueens(1);
    std::vector<sf::Sprite> WhiteKings(1);

    std::vector<sf::Sprite> BlackPawns(8);
    std::vector<sf::Sprite> BlackKnights(2);
    std::vector<sf::Sprite> BlackBishops(2);
    std::vector<sf::Sprite> BlackRooks(2);
    std::vector<sf::Sprite> BlackQueens(8);
    std::vector<sf::Sprite> BlackKings(1);

    // Set the size of each square
    const int squareSize = 100;
    const int boardSize = squareSize * 8;
    const int xOffset = window.getSize().x - boardSize; // Offset for positioning board in the window
    int PieceName;

    // Creating Objects For Dragging Behaviour
    sf::Sprite* draggedPiece = nullptr;
    sf::Vector2f offset;
    sf::Vector2f originalPosition;
  

    // Initialise Piece Sprite Values
    Core::SetupPawns(WhitePawns, t1, xOffset, squareSize, 6); 
    Core::SetupPieces(WhiteKnights, t2, xOffset, squareSize, 7, 1, 6); // (PieceType, Texture, squareSize, Rank, Column1, Column2)
    Core::SetupPieces(WhiteBishops, t3, xOffset, squareSize, 7, 2, 5);
    Core::SetupPieces(WhiteRooks, t4, xOffset, squareSize, 7, 0, 7);
    Core::SetupPieces(WhiteQueens, t5, xOffset, squareSize, 7, 4);
    Core::SetupPieces(WhiteKings, t6, xOffset, squareSize, 7, 3);

    Core::SetupPawns(BlackPawns, t7, xOffset, squareSize, 1);
    Core::SetupPieces(BlackKnights, t8, xOffset, squareSize, 0, 1, 6);
    Core::SetupPieces(BlackBishops, t9, xOffset, squareSize, 0, 2, 5);
    Core::SetupPieces(BlackRooks, t10, xOffset, squareSize, 0, 0, 7);
    Core::SetupPieces(BlackQueens, t11, xOffset, squareSize, 0, 4);
    Core::SetupPieces(BlackKings, t12, xOffset, squareSize, 0, 3);
    

    // Run as long as window is open
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Kill the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Check if the mouse is over any piece
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    // Loop Through Every Piece in the Vector
                    for (auto& pawn : WhitePawns) {
                        if (pawn.getGlobalBounds().contains(mousePos)) { //Check If Mouse Is Held on Piece
                            draggedPiece = &pawn; // Set the Dragged Piece to the Selected Pawn
                            originalPosition = pawn.getPosition();
                            break;
                        }
                    }

                    for (auto& knight : WhiteKnights) {
                        if (knight.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &knight;
                            originalPosition = knight.getPosition();
                            break;
                        }
                    }

                    for (auto& rook : WhiteRooks) {
                        if (rook.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &rook;
                            originalPosition = rook.getPosition();
                            break;
                        }
                    }

                    for (auto& bishop : WhiteBishops) {
                        if (bishop.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &bishop;
                            originalPosition = bishop.getPosition();
                            break;
                        }
                    }

                    for (auto& queen : WhiteQueens) {
                        if (queen.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &queen;
                            originalPosition = queen.getPosition();
                            break;
                        }
                    }

                    for (auto& king : WhiteKings) {
                        if (king.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &king;
                            originalPosition = king.getPosition();
                            break;
                        }
                    }

                    for (auto& pawn : BlackPawns) {
                        if (pawn.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &pawn;
                            originalPosition = pawn.getPosition();
                            break;
                        }
                    }

                    for (auto& knight : BlackKnights) {
                        if (knight.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &knight;
                            originalPosition = knight.getPosition();
                            break;
                        }
                    }

                    for (auto& bishop : BlackBishops) {
                        if (bishop.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &bishop;
                            originalPosition = bishop.getPosition();
                            break;
                        }
                    }

                    for (auto& rook : BlackRooks) {
                        if (rook.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &rook;
                            originalPosition = rook.getPosition();
                            break;
                        }
                    }

                    for (auto& queen : BlackQueens) {
                        if (queen.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &queen;
                            originalPosition = queen.getPosition();
                            break;
                        }
                    }

                    for (auto& king : BlackKings) {
                        if (king.getGlobalBounds().contains(mousePos)) {
                            draggedPiece = &king;
                            originalPosition = king.getPosition();
                            break;
                        }
                    }

                    if (draggedPiece) { // Check If Piece is Selected
                        offset = draggedPiece->getPosition() - mousePos; // Account for Mouse Offset to the position of the Piece
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left && draggedPiece) {
                    // Snap the piece to the nearest square and center it

                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    int col = static_cast<int>((mousePos.x - xOffset) / squareSize); 
                    int row = static_cast<int>(mousePos.y / squareSize);               
                    if (col >= 0 && col < 8 && row >= 0 && row < 8) {
                        sf::Vector2f snappedPosition = Core::getSnappedPosition(row, col, squareSize, xOffset); // Calculate the Nearest Row and Column
                        draggedPiece->setPosition(Core::getCenteredPosition(snappedPosition, *draggedPiece, squareSize)); // on the Grid Compared to the Mouse Position 
                    }
                    else {
                        draggedPiece->setPosition(originalPosition); // Snap back to original position
                    } // Set the Position of the piece to the nearest square
                    draggedPiece = nullptr;  // Stop dragging
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                if (draggedPiece) {
                    // Move the piece with the mouse
                    draggedPiece->setPosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y) + offset);
                }
            }
        }

        // Set Window Background
        window.clear(sf::Color(35,42,45));

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                // Create a rectangle for each square
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

                // Set the position of the square
                square.setPosition(xOffset + col * squareSize, row * squareSize);

                // Alternate colors between Green and white
                if ((row + col) % 2 == 0) {
                    square.setFillColor(sf::Color(238, 238, 210));
                }
                else {
                    square.setFillColor(sf::Color(118, 150, 86));
                }

                // Draw the square
                window.draw(square);
            }
        }


        // Draw the Pieces onto the window
        for (int i = 0; i < 8; ++i) {
            window.draw(WhitePawns[i]);
            window.draw(BlackPawns[i]);
        }

        for (int i = 0; i < 2; ++i) {
            window.draw(WhiteKnights[i]);
            window.draw(WhiteRooks[i]);
            window.draw(WhiteBishops[i]);
            window.draw(BlackKnights[i]);
            window.draw(BlackBishops[i]);
            window.draw(BlackRooks[i]);
        }

        window.draw(WhiteQueens[0]);
        window.draw(WhiteKings[0]);
        window.draw(BlackQueens[0]);
        window.draw(BlackKings[0]);


        window.display();
    }

    return 0;
}