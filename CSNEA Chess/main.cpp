#include <SFML/Graphics.hpp>

int main()
{
    // Create window instance
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Chess");
    sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;

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

    // Set the size of each square
    const int squareSize = 100;
    const int boardSize = squareSize * 8;
    const int xOffset = window.getSize().x - boardSize; // Offset for positioning board in the window

    int PieceName;
    

    // Run as long as window is open
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Kill the window
            if (event.type == sf::Event::Closed)
                window.close();
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


        window.display();
    }

    return 0;
}