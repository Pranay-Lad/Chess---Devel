#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Helpers.h"
#include "Piece.h"
#include "Board.h"


int main()
{
    // Create window instance
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Chess");
    sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;

    sf::RectangleShape menuBar;
    menuBar.setSize(sf::Vector2f(60.0f, 800.f));
    menuBar.setPosition(0.f, 0.f);
    menuBar.setFillColor(sf::Color(193, 255, 193));

    sf::Texture t_ChessIconWhite , t_ChessIconBlack;
    t_ChessIconWhite.loadFromFile("Assets/WhitePawn.png");
    t_ChessIconBlack.loadFromFile("Assets/BlackPawn.png");
    sf::Sprite ChessIcon;
    ChessIcon.setTexture(t_ChessIconWhite);
    ChessIcon.setPosition(0.f, 300.f);
    ChessIcon.setScale(0.5f, 0.5f);
    bool chessIconColour = true;


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
    std::vector<sf::Sprite> BlackQueens(1);
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
    bool isWhiteTurn = true;

    Core::Board Board;
    Board.InitialiseBoard();
    Board.PrintBoard();
  

    // Initialise Piece Sprite Values
    Core::SetupPawns(WhitePawns, t1, xOffset, squareSize, 6);
    Core::SetupPieces(WhiteKnights, t2, xOffset, squareSize, 7, 1, 6);
    Core::SetupPieces(WhiteBishops, t3, xOffset, squareSize, 7, 2, 5);
    Core::SetupPieces(WhiteRooks, t4, xOffset, squareSize, 7, 0, 7);
    Core::SetupPieces(WhiteQueens, t5, xOffset, squareSize, 7, 3);
    Core::SetupPieces(WhiteKings, t6, xOffset, squareSize, 7, 4);

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

                    if (ChessIcon.getGlobalBounds().contains(mousePos)) {
                        if (chessIconColour) {
                            ChessIcon.setTexture(t_ChessIconBlack);
                        }
                        else {
                            ChessIcon.setTexture(t_ChessIconWhite);
                        }
                        chessIconColour = !chessIconColour;
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
                    // Calculate End Column and Row
                    int endCol = static_cast<int>((mousePos.x - xOffset) / squareSize);
                    int endRow = static_cast<int>(mousePos.y / squareSize);

                    // Calculate Start Column and Row
                    int startCol = static_cast<int>((originalPosition.x - xOffset) / squareSize);
                    int startRow = static_cast<int>(originalPosition.y / squareSize);

                    // Initialise boolean validation variables
                    bool isOccupied = false;
                    bool valid = false;
                    bool isMoveValid = false; 
                    bool blocked = true;
                    bool isCaptureWhite = false;
                    bool isCaptureBlack = false;

                    sf::Sprite LastPiece;
                    
                    // Check Turn
                    if (isWhiteTurn) {
                        if (draggedPiece->getTexture() == &t1) { // If the dragged piece has the pawn texture (t1) treat as pawn
                            valid = Piece::isPawnMoveValid(startRow, startCol, endRow, endCol, true, Board.Square); // Validate Pawn Move
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White); // Check if square is Occupied by a Friendly Piece
                            isMoveValid = valid && isOccupied; // Check all conditions are valid
                        }
                        else if (draggedPiece->getTexture() == &t2) {
                            valid = Piece::isKnightMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            isMoveValid = valid && isOccupied;
                        }
                        else if (draggedPiece->getTexture() == &t3) {
                            valid = Piece::isBishopMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            blocked = Board.isBlocked(Core::toIndex(startRow, startCol), Core::toIndex(endRow, endCol)); // Check if path is blocked
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &t4) {
                            valid = Piece::isRookMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            blocked = Board.isBlocked(Core::toIndex(startRow, startCol), Core::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;

                        }
                        else if (draggedPiece->getTexture() == &t5) {
                            valid = Piece::isQueenMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            blocked = Board.isBlocked(Core::toIndex(startRow, startCol), Core::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &t6) {
                            valid = Piece::isKingMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            isMoveValid = valid && isOccupied;
                        }
                    }
                    else {
                        if (draggedPiece->getTexture() == &t7) { // If the dragged piece has the black pawn texture (t8) treat as black pawn
                            valid = Piece::isPawnMoveValid(startRow, startCol, endRow, endCol, false, Board.Square);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            isMoveValid = valid && isOccupied;
                        }
                        else if (draggedPiece->getTexture() == &t8) {
                            valid = Piece::isKnightMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            isMoveValid = valid && isOccupied;
                        }
                        else if (draggedPiece->getTexture() == &t9) {
                            valid = Piece::isBishopMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            blocked = Board.isBlocked(Core::toIndex(startRow, startCol), Core::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &t10) {
                            valid = Piece::isRookMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            blocked = Board.isBlocked(Core::toIndex(startRow, startCol), Core::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;

                        }
                        else if (draggedPiece->getTexture() == &t11) {
                            valid = Piece::isQueenMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            blocked = Board.isBlocked(Core::toIndex(startRow, startCol), Core::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &t12) {
                            valid = Piece::isKingMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            isMoveValid = valid && isOccupied;
                        }
                    }

                    if (isMoveValid && Piece::isPositionValid(endRow, endCol)) {

                        isCaptureWhite = Piece::isOccupiedHostile(endRow, endCol, Piece::Black, Board.Square);
                        isCaptureBlack = Piece::isOccupiedHostile(endRow, endCol, Piece::White, Board.Square); // Check if Capture is Available
                        if (!isCaptureWhite) {
                            if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::White | Piece::Pawn)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhitePawns, squareSize, xOffset); // Temporaily store captured piece
                                Board.removeAtPosition(endRow, endCol, WhitePawns, squareSize, xOffset); // Remove captured piece
                                std::cout << "REMOVE BLACK PAWN" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::White | Piece::Knight)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteKnights, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteKnights, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::White | Piece::Bishop)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteBishops, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteBishops, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::White | Piece::Rook)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteRooks, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteRooks, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::White | Piece::Queen)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteQueens, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteQueens, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::White | Piece::King)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteKings, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteKings, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                        }
                        else if (!isCaptureBlack) {
                            if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::Black | Piece::Pawn)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackPawns, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackPawns, squareSize, xOffset);
                                std::cout << "REMOVE BLACK PAWN" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::Black | Piece::Knight)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackKnights, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackKnights, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::Black | Piece::Bishop)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackBishops, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackBishops, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::Black | Piece::Rook)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackRooks, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackRooks, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::Black | Piece::Queen)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackQueens, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackQueens, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Core::toIndex(endRow, endCol)] == (Piece::Black | Piece::King)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackKings, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackKings, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                        }
                        sf::Vector2f snappedPosition = Core::getSnappedPosition(endRow, endCol, squareSize, xOffset); // Calculate the Nearest Row and Column
                        draggedPiece->setPosition(Core::getCenteredPosition(snappedPosition, *draggedPiece, squareSize)); // on the Grid Compared to the Mouse Position 
                        int lastStartValue = Board.Square[Core::getIndex(originalPosition, squareSize, xOffset)]; // Temporarily Store the values of the start and target square
                        int lastTargetValue = Board.Square[Core::getIndex(snappedPosition, squareSize, xOffset)];
                        Board.MakeMove(Move(Core::getIndex(originalPosition, squareSize, xOffset), Core::getIndex(snappedPosition, squareSize, xOffset)));
                        bool isWhiteKingCheck = Board.isKingInCheck(WhiteKings, true, squareSize, xOffset);
                        bool isBlackKingCheck = Board.isKingInCheck(BlackKings, false, squareSize, xOffset);
                        if (isWhiteKingCheck && isWhiteTurn) { // White is in Check so Previous Move Invalidated
                            Board.Square[Core::getIndex(originalPosition, squareSize, xOffset)] = lastStartValue; // Unmake the Previous Move
                            Board.Square[Core::getIndex(snappedPosition, squareSize, xOffset)] = lastTargetValue;
                            std::cout << "WHITE IN CHECK" << std::endl;
                            if (LastPiece.getTexture() == &t7) { // Determine What Piece Type Captured Piece is
                                BlackPawns.push_back(LastPiece); // Add Back the Captured Piece
                            }
                            else if (LastPiece.getTexture() == &t8) {
                                BlackKnights.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t9) {
                                BlackBishops.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t10) {
                                BlackRooks.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t11) {
                                BlackQueens.push_back(LastPiece);
                                std::cout << "PUSHBACK QUEEN" << std::endl;
                            }
                            draggedPiece->setPosition(originalPosition); // Return to original Position
                        }
                        else if (isBlackKingCheck && !isWhiteTurn) {
                            Board.Square[Core::getIndex(originalPosition, squareSize, xOffset)] = lastStartValue; // Unmake the Previous Move
                            Board.Square[Core::getIndex(snappedPosition, squareSize, xOffset)] = lastTargetValue;
                            std::cout << "BLACK IN CHECK" << std::endl;
                            if (LastPiece.getTexture() == &t1) {
                                WhitePawns.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t2) {
                                WhiteKnights.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t3) {
                                WhiteBishops.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t4) {
                                WhiteRooks.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &t5) {
                                WhiteQueens.push_back(LastPiece);
                            }
                            draggedPiece->setPosition(originalPosition);
                        }
                        else {
                            std::cout << "NOT IN CHECK" << std::endl;
                            isWhiteTurn = !isWhiteTurn; // Flip turn
                        }

                        std::cout << std::endl;
                        Board.PrintBoard(); // Print Current Board Position
                    }
                    else {
                        draggedPiece->setPosition(originalPosition); // Snap back to original position
                    } 
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
        for (auto& pawn : WhitePawns) {
            window.draw(pawn);
        }

        for (auto& knight : WhiteKnights) {
            window.draw(knight);
        }

        for (auto& bishop : WhiteBishops) {
            window.draw(bishop);
        }

        for (auto& rook : WhiteRooks) {
            window.draw(rook);
        }
        if (WhiteQueens.size()) {
            window.draw(WhiteQueens[0]);
        }

        if (WhiteKings.size()) {
            window.draw(WhiteKings[0]);
        }

        for (auto& pawn : BlackPawns) {
            window.draw(pawn);
        }

        for (auto& knight : BlackKnights) {
            window.draw(knight);
        }

        for (auto& bishop : BlackBishops) {
            window.draw(bishop);
        }

        for (auto& rook : BlackRooks) {
            window.draw(rook);
        }

        if (BlackQueens.size()) {
            window.draw(BlackQueens[0]);
        }

        if (BlackKings.size()) {
            window.draw(BlackKings[0]);
        }

        window.draw(menuBar);
        window.draw(ChessIcon);
        window.display();
    }

    return 0;
}