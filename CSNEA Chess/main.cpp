#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <algorithm>

#include "Helpers.h"
#include "Piece.h"
#include "Board.h"
#include "Evaluate.h"

void ShowPromotion(int row, int col, bool isWhite, std::vector<sf::Sprite>& PromotionPieces, std::vector<sf::Texture>& Textures, int squareSize, int xOffset) {
    if (isWhite) {
        PromotionPieces[0].setTexture(Textures[4]);
        PromotionPieces[0].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(0, col, squareSize, xOffset), PromotionPieces[0], squareSize));
        PromotionPieces[1].setTexture(Textures[3]);
        PromotionPieces[1].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(1, col, squareSize, xOffset), PromotionPieces[1], squareSize));
        PromotionPieces[2].setTexture(Textures[2]);
        PromotionPieces[2].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(2, col, squareSize, xOffset), PromotionPieces[2], squareSize));
        PromotionPieces[3].setTexture(Textures[1]);
        PromotionPieces[3].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(3, col, squareSize, xOffset), PromotionPieces[3], squareSize));
    }
    else {
        PromotionPieces[0].setTexture(Textures[10]);
        PromotionPieces[0].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(4, col, squareSize, xOffset), PromotionPieces[0], squareSize));
        PromotionPieces[1].setTexture(Textures[9]);
        PromotionPieces[1].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(5, col, squareSize, xOffset), PromotionPieces[1], squareSize));
        PromotionPieces[2].setTexture(Textures[8]);
        PromotionPieces[2].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(6, col, squareSize, xOffset), PromotionPieces[2], squareSize));
        PromotionPieces[3].setTexture(Textures[7]);
        PromotionPieces[3].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(7, col, squareSize, xOffset), PromotionPieces[3], squareSize));
    }
}








void mainloop()
{
    // Create window instance
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Chess");
    std::vector<sf::Texture> Textures(12); // Create Array of Textures
    //sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;

    sf::Image icon;
    icon.loadFromFile("Assets/ChessIcon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::RectangleShape menuBar;
    menuBar.setSize(sf::Vector2f(60.0f, 800.f));
    menuBar.setPosition(0.f, 0.f);
    menuBar.setFillColor(sf::Color(193, 255, 193));


    sf::RectangleShape EvaluateButton;
    EvaluateButton.setSize(sf::Vector2f(60.0f, 60));
    EvaluateButton.setPosition(100.f, 100.f);
    EvaluateButton.setFillColor(sf::Color(193, 255, 193));

    sf::RectangleShape TestingButton;
    TestingButton.setSize(sf::Vector2f(60.0f, 60));
    TestingButton.setPosition(100.f, 300.0f);
    TestingButton.setFillColor(sf::Color(229, 116, 116));

    sf::RectangleShape PromotionButton;
    PromotionButton.setSize(sf::Vector2f(60.0f, 60));
    PromotionButton.setPosition(100.f, 500);
    PromotionButton.setFillColor(sf::Color(229, 116, 116));

    sf::Texture t_ChessIconWhite , t_ChessIconBlack;
    t_ChessIconWhite.loadFromFile("Assets/WhitePawn.png");
    t_ChessIconBlack.loadFromFile("Assets/BlackPawn.png");
    sf::Sprite ChessIcon;
    ChessIcon.setTexture(t_ChessIconWhite);
    ChessIcon.setPosition(0.f, 300.f);
    ChessIcon.setScale(0.5f, 0.5f);
    bool chessIconColour = true;

    sf::Texture t_HomeEnabled, t_HomeDisabled;
    t_HomeEnabled.loadFromFile("Assets/HomeIcon.png");
    sf::Sprite HomeIcon;
    HomeIcon.setTexture(t_HomeEnabled);
    HomeIcon.setPosition(4.5f, 230.f);
    HomeIcon.setScale(0.10f, 0.10f);

    sf::Texture t_AnalysisEnabled, t_AnalysisDisabled;
    t_AnalysisEnabled.loadFromFile("Assets/AnalysisIcon.png");
    sf::Sprite AnalysisIcon;
    AnalysisIcon.setTexture(t_AnalysisEnabled);
    AnalysisIcon.setPosition(5.5f, 385.f);
    AnalysisIcon.setScale(0.23f, 0.23f);
    bool evaluationMode = false;


   

    

    // Import Textures For Pieces
    Textures[0].loadFromFile("Assets/WhitePawn.png");
    Textures[1].loadFromFile("Assets/WhiteKnight.png");
    Textures[2].loadFromFile("Assets/WhiteBishop.png");
    Textures[3].loadFromFile("Assets/WhiteRook.png");
    Textures[4].loadFromFile("Assets/WhiteQueen.png");
    Textures[5].loadFromFile("Assets/WhiteKing.png");
    Textures[6].loadFromFile("Assets/BlackPawn.png");
    Textures[7].loadFromFile("Assets/BlackKnight.png");
    Textures[8].loadFromFile("Assets/BlackBishop.png");
    Textures[9].loadFromFile("Assets/BlackRook.png");
    Textures[10].loadFromFile("Assets/BlackQueen.png");
    Textures[11].loadFromFile("Assets/BlackKing.png");

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

    // Create Rectangle Object From Promotion Window
    sf::RectangleShape Promotion;
    Promotion.setSize(sf::Vector2f(squareSize, 4 * squareSize)); // Set Size to 1 square wide, 4 squares long
    Promotion.setFillColor(sf::Color::White); // Set Colour White
    Promotion.setPosition(xOffset + 5 * squareSize, 4 * squareSize); // Snap to Grid Position
    Promotion.setOutlineColor(sf::Color::Black); // Set Outline Colour For Distinction
    Promotion.setOutlineThickness(3.0f); // Set Outline Thickness To Be Visible

    std::vector<sf::Sprite> PromotionPieces(4); // Create vector of Promotion Pieces
    PromotionPieces[0].setTexture(Textures[10]); // Set Texture For Each Piece
    PromotionPieces[0].setScale(0.7f, 0.7f); // Set Size of Each Pieces To Match
    PromotionPieces[0].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(4, 5, squareSize, xOffset), PromotionPieces[0], squareSize));
    PromotionPieces[1].setTexture(Textures[11]); // Snap Pieces To Grid Position
    PromotionPieces[1].setScale(0.7f, 0.7f);
    PromotionPieces[1].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(5, 5, squareSize, xOffset), PromotionPieces[1], squareSize));
    PromotionPieces[2].setTexture(Textures[8]);
    PromotionPieces[2].setScale(0.7f, 0.7f);
    PromotionPieces[2].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(6, 5, squareSize, xOffset), PromotionPieces[2], squareSize));
    PromotionPieces[3].setTexture(Textures[7]);
    PromotionPieces[3].setScale(0.7f, 0.7f);
    PromotionPieces[3].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(7, 5, squareSize, xOffset), PromotionPieces[3], squareSize));
    

    // Creating Objects For Dragging Behaviour
    sf::Sprite* draggedPiece = nullptr;
    sf::Vector2f offset;
    sf::Vector2f originalPosition;
    bool isWhiteTurn = true;
    bool isPromotion = false;

    Core::Board Board;
    Evaluate Eval;
    Board.InitialiseBoard();
    Board.PrintBoard();
    int promotionCol = 0;
    int promotionRow = 0;
  

    // Initialise Piece Sprite Values
    Helpers::SetupPawns(WhitePawns, Textures[0], xOffset, squareSize, 6);
    Helpers::SetupPieces(WhiteKnights, Textures[1], xOffset, squareSize, 7, 1, 6);
    Helpers::SetupPieces(WhiteBishops, Textures[2], xOffset, squareSize, 7, 2, 5);
    Helpers::SetupPieces(WhiteRooks, Textures[3], xOffset, squareSize, 7, 0, 7);
    Helpers::SetupPieces(WhiteQueens, Textures[4], xOffset, squareSize, 7, 3);
    Helpers::SetupPieces(WhiteKings, Textures[5], xOffset, squareSize, 7, 4);

    Helpers::SetupPawns(BlackPawns, Textures[6], xOffset, squareSize, 1);
    Helpers::SetupPieces(BlackKnights, Textures[7], xOffset, squareSize, 0, 1, 6);
    Helpers::SetupPieces(BlackBishops, Textures[8], xOffset, squareSize, 0, 2, 5);
    Helpers::SetupPieces(BlackRooks, Textures[9], xOffset, squareSize, 0, 0, 7);
    Helpers::SetupPieces(BlackQueens, Textures[10], xOffset, squareSize, 0, 3);
    Helpers::SetupPieces(BlackKings, Textures[11], xOffset, squareSize, 0, 4);
    

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
                        std::cout << "PLAY BUTTON PRESSED" << std::endl;
                        ChessIcon.setTexture(t_ChessIconBlack);
                        evaluationMode = false;

                    }

                    if (HomeIcon.getGlobalBounds().contains(mousePos)) {
                        ChessIcon.setTexture(t_ChessIconWhite);
                        evaluationMode = false;
                        std::cout << "HOME BUTTON PRESSED" << std::endl;
                    }

                    if (AnalysisIcon.getGlobalBounds().contains(mousePos)) {
                        ChessIcon.setTexture(t_ChessIconWhite);
                        std::cout << "ANALYSIS BUTTON PRESSED" << std::endl;
                        evaluationMode = true;
                    }

                    if (EvaluateButton.getGlobalBounds().contains(mousePos)) {
                        Eval.convertToEvaluate(Board.Square);
                        Eval.PrintBoard();
                        std::cout << "EVAL: " << Eval.evaluateValue() << std::endl;
                    }

                    if (TestingButton.getGlobalBounds().contains(mousePos)) {
                        /*
                        srand(time(0));
                        std::string piecetype;
                        //std::cin >> piecetype;
                        int x = rand() % 8;
                        int y = rand() % 8;
                        std::cout << "X: " << y << "Y: " << x << std::endl;
                        Board.moveAtPosition(6, 1, BlackPawns, squareSize, xOffset, y, x);
                        Board.PrintBoard();*/
                        if (Board.isCheckmate(1)) {
                            std::cout << "White is in checkmate." << std::endl;
                        }
                        else if (Board.isInCheck(1)) {
                            std::cout << "White is in check." << std::endl;
                        }
                        else {
                            std::cout << "White is safe." << std::endl;
                        }

                        // Example: Check for black's checkmate
                        if (Board.isCheckmate(0)) {
                            std::cout << "Black is in checkmate." << std::endl;
                        }
                        else if (Board.isInCheck(0)) {
                            std::cout << "Black is in check." << std::endl;
                        }
                        else {
                            std::cout << "Black is safe." << std::endl;
                        }
                    }

                    if (PromotionButton.getGlobalBounds().contains(mousePos)) {
                        //std::cout << "PROMOTION COl: " << promotionCol << "PROMOTION ROW" << promotionRow << std::endl;
                        //bool checkmate = isCheckmate(Board.Square, true);
                        //std::cout << "Is black in checkmate? " << (checkmate ? "Yes" : "No") << std::endl;
                    }

                    if (isPromotion) {
                        std::cout << "PROMOTION COl: " << promotionCol << "PROMOTION ROW" << promotionRow << std::endl;
                        std::cout << "PROMOTION INDEX" << Helpers::toIndex(promotionRow, promotionCol) << std::endl;
                        if (PromotionPieces[0].getGlobalBounds().contains(mousePos)) { 
                            std::cout << "QUEEN PROMOTION" << std::endl; 
                            isPromotion = !isPromotion;

                        }
                        if (PromotionPieces[1].getGlobalBounds().contains(mousePos)) { std::cout << "ROOK PROMOTION" << std::endl; isPromotion = !isPromotion;
                        }
                        if (PromotionPieces[2].getGlobalBounds().contains(mousePos)) { std::cout << "BISHOP PROMOTION" << std::endl; isPromotion = !isPromotion;
                        }
                        if (PromotionPieces[3].getGlobalBounds().contains(mousePos)) { std::cout << "KNIGHT PROMOTION" << std::endl; isPromotion = !isPromotion;
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
                    bool whiteCastled = false;
                    bool blackCastled = false;
                    bool isPromotionAllowed = false;

                    sf::Sprite LastPiece;
                    
                    // Check Turn
                    if (isWhiteTurn) {
                        if (draggedPiece->getTexture() == &Textures[0]) { // If the dragged piece has the pawn texture (t1) treat as pawn
                            valid = Piece::isPawnMoveValid(startRow, startCol, endRow, endCol, true, Board.Square); // Validate Pawn Move
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White); // Check if square is Occupied by a Friendly Piece
                            isMoveValid = valid && isOccupied; // Check all conditions are valid
                            isPromotionAllowed = Piece::isPromotion(startRow, startCol, endRow, endCol, true, Board.Square);
                        }
                        else if (draggedPiece->getTexture() == &Textures[1]) {
                            valid = Piece::isKnightMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            isMoveValid = valid && isOccupied;
                        }
                        else if (draggedPiece->getTexture() == &Textures[2]) {
                            valid = Piece::isBishopMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            blocked = Board.isBlocked(Helpers::toIndex(startRow, startCol), Helpers::toIndex(endRow, endCol)); // Check if path is blocked
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &Textures[3]) {
                            valid = Piece::isRookMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            blocked = Board.isBlocked(Helpers::toIndex(startRow, startCol), Helpers::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;

                        }
                        else if (draggedPiece->getTexture() == &Textures[4]) {
                            valid = Piece::isQueenMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            blocked = Board.isBlocked(Helpers::toIndex(startRow, startCol), Helpers::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &Textures[5]) {
                            valid = Piece::isKingMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::White);
                            isMoveValid = valid && isOccupied;
                        }
                    }
                    else {
                        if (draggedPiece->getTexture() == &Textures[6]) { // If the dragged piece has the black pawn texture (t8) treat as black pawn
                            valid = Piece::isPawnMoveValid(startRow, startCol, endRow, endCol, false, Board.Square);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            isMoveValid = valid && isOccupied;
                            isPromotionAllowed = Piece::isPromotion(startRow, startCol, endRow, endCol, false , Board.Square);
                        }
                        else if (draggedPiece->getTexture() == &Textures[7]) {
                            valid = Piece::isKnightMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            isMoveValid = valid && isOccupied;
                        }
                        else if (draggedPiece->getTexture() == &Textures[8]) {
                            valid = Piece::isBishopMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            blocked = Board.isBlocked(Helpers::toIndex(startRow, startCol), Helpers::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &Textures[9]) {
                            valid = Piece::isRookMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            blocked = Board.isBlocked(Helpers::toIndex(startRow, startCol), Helpers::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;

                        }
                        else if (draggedPiece->getTexture() == &Textures[10]) {
                            valid = Piece::isQueenMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            blocked = Board.isBlocked(Helpers::toIndex(startRow, startCol), Helpers::toIndex(endRow, endCol));
                            isMoveValid = valid && isOccupied && !blocked;
                        }
                        else if (draggedPiece->getTexture() == &Textures[11]) {
                            valid = Piece::isKingMoveValid(startRow, startCol, endRow, endCol);
                            isOccupied = Board.isOccupiedFriendly(endRow, endCol, Piece::Black);
                            isMoveValid = valid && isOccupied;
                        }
                    }

                    if (isMoveValid && Piece::isPositionValid(endRow, endCol)) {
                        
                        isCaptureWhite = Piece::isOccupiedHostile(endRow, endCol, Piece::Black, Board.Square);
                        isCaptureBlack = Piece::isOccupiedHostile(endRow, endCol, Piece::White, Board.Square); // Check if Capture is Available
                        if (!isCaptureWhite) {
                            if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Pawn)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhitePawns, squareSize, xOffset); // Temporaily store captured piece
                                Board.removeAtPosition(endRow, endCol, WhitePawns, squareSize, xOffset); // Remove captured piece
                                std::cout << "REMOVE BLACK PAWN" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Knight)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteKnights, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteKnights, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Bishop)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteBishops, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteBishops, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Rook)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteRooks, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteRooks, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Queen)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteQueens, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteQueens, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::King)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteKings, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteKings, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                        }
                        else if (!isCaptureBlack) {
                            if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Pawn)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackPawns, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackPawns, squareSize, xOffset);
                                std::cout << "REMOVE BLACK PAWN" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Knight)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackKnights, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackKnights, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Bishop)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackBishops, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackBishops, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Rook)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackRooks, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackRooks, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Queen)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackQueens, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackQueens, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::King)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackKings, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackKings, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            
                        }
                        sf::Vector2f snappedPosition = Helpers::getSnappedPosition(endRow, endCol, squareSize, xOffset); // Calculate the Nearest Row and Column
                        draggedPiece->setPosition(Helpers::getCenteredPosition(snappedPosition, *draggedPiece, squareSize)); // on the Grid Compared to the Mouse Position 
                        int lastStartValue = Board.Square[Helpers::getIndex(originalPosition, squareSize, xOffset)]; // Temporarily Store the values of the start and target square
                        int lastTargetValue = Board.Square[Helpers::getIndex(snappedPosition, squareSize, xOffset)];
                        Board.MakeMove(Move(Helpers::getIndex(originalPosition, squareSize, xOffset), Helpers::getIndex(snappedPosition, squareSize, xOffset)));
                        bool isWhiteKingCheck = Board.isKingInCheck(WhiteKings, true, squareSize, xOffset);
                        bool isBlackKingCheck = Board.isKingInCheck(BlackKings, false, squareSize, xOffset);
                        if (isWhiteKingCheck && isWhiteTurn) { // White is in Check so Previous Move Invalidated
                            Board.Square[Helpers::getIndex(originalPosition, squareSize, xOffset)] = lastStartValue; // Unmake the Previous Move
                            Board.Square[Helpers::getIndex(snappedPosition, squareSize, xOffset)] = lastTargetValue;
                            std::cout << "WHITE IN CHECK" << std::endl;
                            if (LastPiece.getTexture() == &Textures[6]) { // Determine What Piece Type Captured Piece is
                                BlackPawns.push_back(LastPiece); // Add Back the Captured Piece
                            }
                            else if (LastPiece.getTexture() == &Textures[7]) {
                                BlackKnights.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[8]) {
                                BlackBishops.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[9]) {
                                BlackRooks.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[10]) {
                                BlackQueens.push_back(LastPiece);
                                std::cout << "PUSHBACK QUEEN" << std::endl;
                            }
                            draggedPiece->setPosition(originalPosition); // Return to original Position
                        }
                        else if (isBlackKingCheck && !isWhiteTurn) {
                            Board.Square[Helpers::getIndex(originalPosition, squareSize, xOffset)] = lastStartValue; // Unmake the Previous Move
                            Board.Square[Helpers::getIndex(snappedPosition, squareSize, xOffset)] = lastTargetValue;
                            std::cout << "BLACK IN CHECK" << std::endl;
                            if (LastPiece.getTexture() == &Textures[0]) {
                                WhitePawns.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[1]) {
                                WhiteKnights.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[2]) {
                                WhiteBishops.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[3]) {
                                WhiteRooks.push_back(LastPiece);
                            }
                            else if (LastPiece.getTexture() == &Textures[4]) {
                                WhiteQueens.push_back(LastPiece);
                            }
                            draggedPiece->setPosition(originalPosition);
                        }
                        else {

                            // If Piece Played in Pawn on Promotion Square, Call Promotion Function

                            if (isPromotionAllowed) {
                                std::cout << "PAWN CAN PROMOTE" << std::endl;
                                isPromotion = true;
                                
                                if (isWhiteTurn) {
                                    Promotion.setPosition(xOffset + (endCol) * squareSize, 0 * squareSize);
                                    ShowPromotion(5, endCol, isWhiteTurn, PromotionPieces, Textures, squareSize, xOffset);
                                    promotionRow = endRow;
                                    promotionCol = endCol;
                                    
                                }
                                else  {
                                    Promotion.setPosition(xOffset + (endCol) * squareSize, 4 * squareSize);
                                    ShowPromotion(3, endCol, isWhiteTurn, PromotionPieces, Textures, squareSize, xOffset);
                                    promotionRow = endRow;
                                    promotionCol = endCol;
                                }
                                
                                
                                
                            }
                            else {
                                std::cout << "PAWN CANNOT PROMOTE" << std::endl;
                            }
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
        window.draw(HomeIcon);
        window.draw(AnalysisIcon);

        if (isPromotion) {
            window.draw(Promotion);
            for (auto& piece : PromotionPieces) {
                window.draw(piece);
            }
        }
        
        
        if (evaluationMode) {
            window.draw(EvaluateButton);
            window.draw(TestingButton);
            window.draw(PromotionButton);
        }
        
        window.display();
    }

    
}

int main() {
    mainloop();
    return 0;
}