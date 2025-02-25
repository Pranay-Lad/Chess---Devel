#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <algorithm>
#include <sstream>
#include <msclr/marshal_cppstd.h>


#include "Helpers.h"
#include "Piece.h"
#include "Board.h"
#include "Evaluate.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;

// Set up Promotion Textures and Position
void ShowPromotion(int row, int col, bool isWhite, std::vector<sf::Sprite>& PromotionPieces, std::vector<sf::Texture>& Textures, int squareSize, int xOffset) {
    if (isWhite) {
        PromotionPieces[0].setTexture(Textures[4]); // Set White Queen ; Snap To Grid Position
        PromotionPieces[0].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(0, col, squareSize, xOffset), PromotionPieces[0], squareSize));
        PromotionPieces[1].setTexture(Textures[3]); // Snap to Row Below
        PromotionPieces[1].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(1, col, squareSize, xOffset), PromotionPieces[1], squareSize));
        PromotionPieces[2].setTexture(Textures[2]);
        PromotionPieces[2].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(2, col, squareSize, xOffset), PromotionPieces[2], squareSize));
        PromotionPieces[3].setTexture(Textures[1]);
        PromotionPieces[3].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(3, col, squareSize, xOffset), PromotionPieces[3], squareSize));
    }
    else {
        PromotionPieces[0].setTexture(Textures[10]); // Set Black Queen
        PromotionPieces[0].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(4, col, squareSize, xOffset), PromotionPieces[0], squareSize));
        PromotionPieces[1].setTexture(Textures[9]); // Snap To Row Above
        PromotionPieces[1].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(5, col, squareSize, xOffset), PromotionPieces[1], squareSize));
        PromotionPieces[2].setTexture(Textures[8]);
        PromotionPieces[2].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(6, col, squareSize, xOffset), PromotionPieces[2], squareSize));
        PromotionPieces[3].setTexture(Textures[7]);
        PromotionPieces[3].setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(7, col, squareSize, xOffset), PromotionPieces[3], squareSize));
    }
}

// Move Generation
std::vector<std::pair<int, int>> GenerateLegalMoves(int board[64], int color) {
    std::vector<std::pair<int, int>> legalMoves;

    // Direction vectors for sliding pieces (rook, bishop, queen)
    const int rookDirections[4] = { -8, 8, -1, 1 }; // Up, down, left, right
    const int bishopDirections[4] = { -9, -7, 7, 9 }; // Diagonals
    const int knightOffsets[8] = { -17, -15, -10, -6, 6, 10, 15, 17 }; // Knight moves
    const int kingOffsets[8] = { -9, -8, -7, -1, 1, 7, 8, 9 }; // King moves

    int pawnDirection = (color == 1) ? -8 : 8; // Forward movement direction for pawns
    int opponentColorStart = (color == 1) ? 9 : 1;
    int opponentColorEnd = (color == 1) ? 14 : 6;

    for (int i = 0; i < 64; i++) {
        int piece = board[i];
        if (piece == 0 || (color == 1 && piece > 6) || (color == 2 && piece < 9)) {
            continue; // Skip empty squares and opponent pieces
        }

        if (piece == 1 || piece == 9) { // Pawn
            int forward = i + pawnDirection;
            if (forward >= 0 && forward < 64 && board[forward] == 0) {
                legalMoves.emplace_back(i, forward);
                if ((color == 1 && i / 8 == 6) || (color == 2 && i / 8 == 1)) { // Double move
                    int doubleForward = forward + pawnDirection;
                    if (board[doubleForward] == 0) {
                        legalMoves.emplace_back(i, doubleForward);
                    }
                }
            }

            // Diagonal captures
            int captureLeft = forward - 1;
            int captureRight = forward + 1;
            if (captureLeft >= 0 && captureLeft < 64 && i % 8 != 0 &&
                board[captureLeft] >= opponentColorStart && board[captureLeft] <= opponentColorEnd) {
                legalMoves.emplace_back(i, captureLeft);
            }
            if (captureRight >= 0 && captureRight < 64 && i % 8 != 7 &&
                board[captureRight] >= opponentColorStart && board[captureRight] <= opponentColorEnd) {
                legalMoves.emplace_back(i, captureRight);
            }
        }
        else if (piece == 2 || piece == 10) { // Knight
            for (int offset : knightOffsets) {
                int target = i + offset;
                if (target >= 0 && target < 64 && (abs((i % 8) - (target % 8)) <= 2)) {
                    if (board[target] == 0 || (board[target] >= opponentColorStart && board[target] <= opponentColorEnd)) {
                        legalMoves.emplace_back(i, target);
                    }
                }
            }
        }
        else if (piece == 3 || piece == 11 || piece == 5 || piece == 13) { // Bishop or Queen
            const int* directions = bishopDirections;
            int numDirections = 4;
            if (piece == 5 || piece == 13) { // Queen also moves like a rook
                numDirections = 8;
            }
            for (int d = 0; d < numDirections; d++) {
                int current = i;
                while (true) {
                    current += directions[d];
                    if (current < 0 || current >= 64 || abs((current % 8) - (i % 8)) > 1) break;
                    if (board[current] == 0) {
                        legalMoves.emplace_back(i, current);
                    }
                    else if (board[current] >= opponentColorStart && board[current] <= opponentColorEnd) {
                        legalMoves.emplace_back(i, current);
                        break;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        else if (piece == 4 || piece == 12) { // Rook
            for (int d = 0; d < 4; d++) {
                int current = i;
                while (true) {
                    current += rookDirections[d];
                    if (current < 0 || current >= 64 || abs((current % 8) - (i % 8)) > 1) break;
                    if (board[current] == 0) {
                        legalMoves.emplace_back(i, current);
                    }
                    else if (board[current] >= opponentColorStart && board[current] <= opponentColorEnd) {
                        legalMoves.emplace_back(i, current);
                        break;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        else if (piece == 6 || piece == 14) { // King
            for (int offset : kingOffsets) {
                int target = i + offset;
                if (target >= 0 && target < 64 && abs((i % 8) - (target % 8)) <= 1) {
                    if (board[target] == 0 || (board[target] >= opponentColorStart && board[target] <= opponentColorEnd)) {
                        legalMoves.emplace_back(i, target);
                    }
                }
            }
        }
    }

    return legalMoves;
}

using namespace std;
string IndexToNotation(int index) {
    char file = 'A' + (index % 8);  // Column letter
    char rank = '8' - (index / 8); // Row number
    return string(1, file) + rank;
}

// Returns the piece name
string PieceName(int piece) {
    switch (piece) {
    case 1: case 9: return "Pawn";
    case 2: case 10: return "Knight";
    case 3: case 11: return "Bishop";
    case 4: case 12: return "Rook";
    case 5: case 13: return "Queen";
    case 6: case 14: return "King";
    default: return "Unknown";
    }
}

const int EMPTY = 0;
const int WHITE_KING = 6;
const int BLACK_KING = 14;

void MakeMove(int board[64], int from, int to, int& capturedPiece) {
    capturedPiece = board[to];
    board[to] = board[from];
    board[from] = EMPTY;
}

// Reverts a simulated move
void UndoMove(int board[64], int from, int to, int capturedPiece) {
    board[from] = board[to];
    board[to] = capturedPiece;
}

// Function For Checkmate
bool isCheckmate(Core::Board board, int colour, std::vector<sf::Sprite>& king, int squareSize, int xOffset) {
    int kingPosition = -1;
    bool isWhiteColour;
    // Set Colour Boolean Variable
    if (colour == 1) {
        isWhiteColour = true;
    }
    else {
        isWhiteColour = false;
    }
    for (int i = 0; i < 64; i++) { // Find King Position
        if ((colour == 1 && board.Square[i] == WHITE_KING) || (colour == 2 && board.Square[i] == BLACK_KING)) {
            kingPosition = i;
            break;
        }
    }
    
    
    // Check if the king is in check
    if (!board.isKingInCheck(king, isWhiteColour, squareSize, xOffset)) {
        cout << "The king is not in check.\n";
        return false;
    }

    // Generate all legal moves
    vector<pair<int, int>> legalMoves = GenerateLegalMoves(board.Square, colour);

    // If no legal moves exist, it's checkmate
    if (legalMoves.empty()) {
        cout << "No legal moves available. It's checkmate.\n";
        return true;
    }

    // Output all legal moves
    int capturedPiece;
    cout << "Available moves for color " << (colour == 1 ? "White" : "Black") << ":\n";
    for (const auto& move : legalMoves) {
        MakeMove(board.Square, move.first, move.second, capturedPiece);

        // Check if the king is still in check
        if (!board.isKingInCheck(king, isWhiteColour, squareSize, xOffset)) {
            cout << PieceName(board.Square[move.first]) << " from " << IndexToNotation(move.first)
                << " to " << IndexToNotation(move.second) << "\n";
        }

        UndoMove(board.Square, move.first, move.second, capturedPiece);
    }

    return false;

}

// Convert Integer Seconds to String to Display
std::string int_to_Time(int time) {
    // Format - xx:xx
    // Ensure 4 digit time by adding 0 where necessary
    std::string minutes = ((time / 60) > 9) ? std::to_string(time / 60) : "0" + std::to_string(time / 60);
    std::string seconds = ((time % 60) > 9) ? std::to_string(time % 60) : "0" + std::to_string(time % 60);
    std::string conv_Time = (minutes) + ":" + (seconds);
    //std::cout << conv_Time << std::endl;
    return conv_Time;
}

// Structure to hold player data
struct Player {
    std::string name;
    int wins;
    int losses;
    int draws;
};

// Database
std::vector<Player> FetchPlayerData() {
    std::vector<Player> players;

    String^ connectionString =
        "Data Source=(LocalDB)\\MSSQLLocalDB;"
        "AttachDbFilename=C:\\USERS\\LADPR\\DOCUMENTS\\CHESSDATABASE.mdf;"
        "Integrated Security=True;";

    try {
        SqlConnection^ connection = gcnew SqlConnection(connectionString);
        connection->Open();

        String^ sqlQuery = "SELECT PlayerName, Wins, Losses, Draws FROM Players";
        SqlCommand^ command = gcnew SqlCommand(sqlQuery, connection);
        SqlDataReader^ reader = command->ExecuteReader();

        while (reader->Read()) {
            Player p;
            p.name = msclr::interop::marshal_as<std::string>(reader["PlayerName"]->ToString());
            p.wins = reader->GetInt32(1);
            p.losses = reader->GetInt32(2);
            p.draws = reader->GetInt32(3);

            players.push_back(p);
        }

        connection->Close();
    }
    catch (SqlException^ ex) {
        Console::WriteLine("SQL Error: {0}", ex->Message);
    }

    return players;
}

// Function to create SFML window and display data
void ShowDatabaseWindow() {
    std::vector<Player> players = FetchPlayerData();

    sf::RenderWindow window(sf::VideoMode(600, 400), "Chess Database");

    sf::Font font;
    font.loadFromFile("C:\\Users\\ladpr\\Fonts\\RobotoMono-Bold.ttf");


    std::vector<sf::Text> textList;
    float yOffset = 50;

    for (const auto& player : players) {
        std::ostringstream oss;
        oss << "Name: " << player.name << " | Wins: " << player.wins
            << " | Losses: " << player.losses << " | Draws: " << player.draws;

        sf::Text text;
        text.setFont(font);
        text.setString(oss.str());
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(20, yOffset);

        textList.push_back(text);
        yOffset += 30; // Move down for the next player
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (const auto& text : textList) {
            window.draw(text);
        }

        window.display();
    }
}



// Function for opening the login window
void openLoginWindow() {
    sf::RenderWindow loginWindow(sf::VideoMode(340, 50), "Login Window", sf::Style::None); // Create window
    loginWindow.setPosition(sf::Vector2i(750, 500)); // change position
    // Load font
    sf::Font font;
    font.loadFromFile("C:\\Users\\ladpr\\Fonts\\RobotoMono-Bold.ttf");

    // Input box (rectangle)
    sf::RectangleShape inputBox(sf::Vector2f(340, 50));
    //inputBox.setPosition(200, 250);
    inputBox.setPosition(0, 0);
    inputBox.setFillColor(sf::Color::Black);

    // Text inside input box
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(10, 10); // Inside the box

    std::string userInput;

    while (loginWindow.isOpen()) {
        sf::Event event;
        while (loginWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                loginWindow.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << userInput << std::endl;
                    loginWindow.close();
                }
            }

            // Handle text input
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) { // Only allow ASCII characters
                    if (event.text.unicode == '\b' && !userInput.empty()) { // Handle backspace
                        userInput.pop_back();
                    }
                    else if (event.text.unicode >= 32 && event.text.unicode <= 126) { // Printable characters
                        userInput += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(userInput);
                }
            }
        }

        loginWindow.clear(sf::Color::Blue);
        loginWindow.draw(inputBox);
        loginWindow.draw(inputText);
        loginWindow.display();
    }
}
void mainloop()
{
    bool is_login = true;
    // Create window instance
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Chess");
    
    std::vector<sf::Texture> Textures(12); // Create Array of Textures
    
    


    int x = 120;

    // *Timer*
    
    int WhiteTimeRemaining = 3600; // 10 Minute Values
    int BlackTimeRemaining = 3600;
    // Fonts
    sf::Font f_roboto;
    f_roboto.loadFromFile("C:\\Users\\ladpr\\Fonts\\RobotoMono-Bold.ttf");
    sf::Text whiteTime;
    sf::Text blackTime;
    blackTime.setFont(f_roboto);
    blackTime.setString(int_to_Time(BlackTimeRemaining));
    // Setup Values
    blackTime.setFillColor(sf::Color::White);
    blackTime.setPosition(sf::Vector2f(250.0f, 336.0f));
    whiteTime.setFont(f_roboto);
    whiteTime.setString(int_to_Time(BlackTimeRemaining));
    whiteTime.setFillColor(sf::Color::Black);
    whiteTime.setPosition(sf::Vector2f(250.0f, 436.0f));

    sf::Clock clock; // Initialise Clock


    // Timer Box
    sf::RectangleShape WhiteTimeBox;
    sf::RectangleShape BlackTimeBox;
    BlackTimeBox.setFillColor(sf::Color::Black); // Setup Values
    WhiteTimeBox.setFillColor(sf::Color::White);
    BlackTimeBox.setSize(sf::Vector2f(190.0f, 100.0f));
    BlackTimeBox.setPosition(sf::Vector2f(200.0f, 300.0f));
    WhiteTimeBox.setSize(sf::Vector2f(190.0f, 100.0f));
    WhiteTimeBox.setPosition(sf::Vector2f(200.0f, 400.0f));

    // Set Icon
    sf::Image icon;
    icon.loadFromFile("Assets/ChessIcon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    // Menu Bar
    sf::RectangleShape menuBar;
    menuBar.setSize(sf::Vector2f(60.0f, 800.f));
    menuBar.setPosition(0.f, 0.f);
    menuBar.setFillColor(sf::Color(193, 255, 193));

    // Evaluation Button
    sf::RectangleShape EvaluateButton;
    EvaluateButton.setSize(sf::Vector2f(60.0f, 60));
    EvaluateButton.setPosition(100.f, 100.f);
    EvaluateButton.setFillColor(sf::Color(193, 255, 193));

    // Testing Button
    sf::RectangleShape TestingButton;
    TestingButton.setSize(sf::Vector2f(60.0f, 60));
    TestingButton.setPosition(100.f, 300.0f);
    TestingButton.setFillColor(sf::Color(229, 116, 116));

    // Promotion Button
    sf::RectangleShape PromotionButton;
    PromotionButton.setSize(sf::Vector2f(60.0f, 60));
    PromotionButton.setPosition(100.f, 500);
    PromotionButton.setFillColor(sf::Color(229, 116, 116));

    // Play MenuBar Button
    sf::Texture t_ChessIconWhite , t_ChessIconBlack;
    t_ChessIconWhite.loadFromFile("Assets/WhitePawn.png");
    t_ChessIconBlack.loadFromFile("Assets/BlackPawn.png");
    sf::Sprite ChessIcon;
    ChessIcon.setTexture(t_ChessIconWhite);
    ChessIcon.setPosition(0.f, 300.f);
    ChessIcon.setScale(0.5f, 0.5f);
    bool chessIconColour = true;
    bool playGameMode = false;

    // Play Game Button
    sf::RectangleShape playGame;
    playGame.setFillColor(sf::Color(229, 116, 116));
    playGame.setPosition(sf::Vector2f(200.0f, 160.0f));
    playGame.setSize(sf::Vector2f(190.0f, 100.0f));
    sf::Text playGameText;
    playGameText.setFont(f_roboto);
    playGameText.setFillColor(sf::Color::Black);
    playGameText.setPosition(sf::Vector2f(220.0f, 200.0f));
    playGameText.setScale(sf::Vector2f(0.8f, 0.8f));
    playGameText.setString("START GAME");


    // Home MenuBar Button
    sf::Texture t_HomeEnabled, t_HomeDisabled;
    t_HomeEnabled.loadFromFile("Assets/HomeIcon.png");
    sf::Sprite HomeIcon;
    HomeIcon.setTexture(t_HomeEnabled);
    HomeIcon.setPosition(4.5f, 230.f);
    HomeIcon.setScale(0.10f, 0.10f);

    // Analysis MenuBar Button  
    sf::Texture t_AnalysisEnabled, t_AnalysisDisabled;
    t_AnalysisEnabled.loadFromFile("Assets/AnalysisIcon.png");
    sf::Sprite AnalysisIcon;
    AnalysisIcon.setTexture(t_AnalysisEnabled);
    AnalysisIcon.setPosition(5.5f, 385.f);
    AnalysisIcon.setScale(0.23f, 0.23f);
    bool evaluationMode = false;


    // Evaluation Bar
    sf::RectangleShape WhiteBar;
    sf::RectangleShape BlackBar;
    // Setup POsition and Size Values
    WhiteBar.setSize(sf::Vector2f(40.0f, 800.0f));
    WhiteBar.setPosition(sf::Vector2f(450.0f, 0.0f));
    WhiteBar.setFillColor(sf::Color::White);
    BlackBar.setSize(sf::Vector2f(40.0f, 400.0f));
    BlackBar.setPosition(sf::Vector2f(450.0f, 0.0f));
    BlackBar.setFillColor(sf::Color::Black);


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

    // Initialse Board Values
    Core::Board Board;
    Evaluate Eval;
    Board.InitialiseBoard();
    Board.PrintBoard();
    int promotionCol = 0;
    int promotionRow = 0;
    int currentTime = 0;
    int elapsedTime = 0;
    bool gameStart = false;

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


    //DataBase

    String^ connectionString =
        "Data Source=(LocalDB)\\MSSQLLocalDB;"
        "AttachDbFilename=C:\\USERS\\LADPR\\DOCUMENTS\\CHESSDATABASE.mdf;"
        "Integrated Security=True;";
    
    try
    {
        // Create the connection
        SqlConnection^ connection = gcnew SqlConnection(connectionString);
        connection->Open();
        Console::WriteLine("Connection to database succeeded!");
        /*
        String^ sqlQuery = "INSERT INTO Players (PlayerName, Wins, Losses, Draws) VALUES (@name, @wins, @losses, @draws)";
        SqlCommand^ command = gcnew SqlCommand(sqlQuery, connection);
        command->Parameters->AddWithValue("@name", "Philip");
        command->Parameters->AddWithValue("@wins", 8);
        command->Parameters->AddWithValue("@losses", 20);
        command->Parameters->AddWithValue("@draws", 4);

        int rowsAffected = command->ExecuteNonQuery();
        Console::WriteLine("Rows Inserted: {0}", rowsAffected);
        */
        connection->Close();

    }
    catch (SqlException^ ex)
    {
        Console::WriteLine("SQL Error: {0}", ex->Message);
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("Error: {0}", ex->Message);
    }

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
                        playGameMode = true;

                    }

                    if (HomeIcon.getGlobalBounds().contains(mousePos)) {
                        ChessIcon.setTexture(t_ChessIconWhite);
                        evaluationMode = false;
                        playGameMode = false;
                        std::cout << "HOME BUTTON PRESSED" << std::endl;
                    }

                    if (AnalysisIcon.getGlobalBounds().contains(mousePos)) {
                        ChessIcon.setTexture(t_ChessIconWhite);
                        std::cout << "ANALYSIS BUTTON PRESSED" << std::endl;
                        evaluationMode = true;
                        playGameMode = false;
                    }

                    if (EvaluateButton.getGlobalBounds().contains(mousePos)) {
                        Eval.convertToEvaluate(Board.Square);
                        Eval.PrintBoard();
                        std::cout << "EVAL: " << Eval.evaluateValue() << std::endl;
                    }

                    if (TestingButton.getGlobalBounds().contains(mousePos)) {
                        ShowDatabaseWindow();
                        
                    }

                    if (PromotionButton.getGlobalBounds().contains(mousePos)) {
                        Eval.convertToEvaluate(Board.Square);
                        Eval.evaluationBar(BlackBar);
                    }


                    if (playGame.getGlobalBounds().contains(mousePos)) {
                        std::cout << "START GAME " << std::endl;
                        clock.restart();
                        gameStart = true;
                    }
                    // Check If In Promotion Mode
                    if (isPromotion) {
                        !isWhiteTurn ? std::cout << "WHITE TURN" << std::endl : std::cout << "BLACK TURN" << std::endl;
                        int promotionColour = isWhiteTurn ? 8 : 0; // Set Colour Offset
                        if (PromotionPieces[0].getGlobalBounds().contains(mousePos)) { 
                            std::cout << "QUEEN PROMOTION" << std::endl;
                            sf::Sprite temp = PromotionPieces[0]; // Set New Piece To A Promotion Piece -> Size and Texture Setup
                            temp.setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(promotionRow, promotionCol, squareSize, xOffset), temp, squareSize));
                            // Set Position of Promoted Piece
                            isWhiteTurn ? BlackQueens.push_back(temp) : WhiteQueens.push_back(temp); // Add Piece To Relevant Vector
                            Board.Square[Helpers::toIndex(promotionRow, promotionCol)] = promotionColour + 5; // Update Internal Board
                            isPromotion = !isPromotion; // Toggle Promotion Mode

                        }
                        if (PromotionPieces[1].getGlobalBounds().contains(mousePos)) { 
                            std::cout << "ROOK PROMOTION" << std::endl; 
                            sf::Sprite temp = PromotionPieces[1];
                            temp.setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(promotionRow, promotionCol, squareSize, xOffset), temp, squareSize));
                            isWhiteTurn ? BlackRooks.push_back(temp) : WhiteRooks.push_back(temp);
                            Board.Square[Helpers::toIndex(promotionRow, promotionCol)] = promotionColour + 4;
                            isPromotion = !isPromotion;
                        }
                        if (PromotionPieces[2].getGlobalBounds().contains(mousePos)) { 
                            std::cout << "BISHOP PROMOTION" << std::endl; 
                            sf::Sprite temp = PromotionPieces[2];
                            temp.setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(promotionRow, promotionCol, squareSize, xOffset), temp, squareSize));
                            isWhiteTurn ? BlackBishops.push_back(temp) : WhiteBishops.push_back(temp);
                            Board.Square[Helpers::toIndex(promotionRow, promotionCol)] = promotionColour + 3;
                            isPromotion = !isPromotion;
                        }
                        if (PromotionPieces[3].getGlobalBounds().contains(mousePos)) { 
                            std::cout << "KNIGHT PROMOTION" << std::endl; 
                            sf::Sprite temp = PromotionPieces[3];
                            temp.setPosition(Helpers::getCenteredPosition(Helpers::getSnappedPosition(promotionRow, promotionCol, squareSize, xOffset), temp, squareSize));
                            isWhiteTurn ? BlackKnights.push_back(temp) : WhiteKnights.push_back(temp);
                            Board.Square[Helpers::toIndex(promotionRow, promotionCol)] = promotionColour + 1;
                            isPromotion = !isPromotion;
                        }
                        Board.PrintBoard();
                        Eval.convertToEvaluate(Board.Square); // Evaluate Board
                        Eval.evaluationBar(BlackBar); // Update Bar
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
                            isPromotionAllowed = Piece::isPromotion(startRow, startCol, endRow, endCol, true, Board.Square); // Check If Promotion
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
                            valid = Piece::isKingMoveValid(startRow, startCol, endRow, endCol, false, Board.Square, true);
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
                            valid = Piece::isKingMoveValid(startRow, startCol, endRow, endCol, false, Board.Square, false);
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
                                std::cout << "REMOVE White PAWN" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Knight)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteKnights, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteKnights, squareSize, xOffset);
                                std::cout << "REMOVE White Knight" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Bishop)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteBishops, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteBishops, squareSize, xOffset);
                                std::cout << "REMOVE White Bishop" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Rook)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteRooks, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteRooks, squareSize, xOffset);
                                std::cout << "REMOVE White Rook" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::Queen)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteQueens, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteQueens, squareSize, xOffset);
                                std::cout << "REMOVE White Queen" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::White | Piece::King)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, WhiteKings, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, WhiteKings, squareSize, xOffset);
                                std::cout << "REMOVE White King" << std::endl;
                            }
                        }
                        else if (!isCaptureBlack) {
                            if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Pawn)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackPawns, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackPawns, squareSize, xOffset);
                                std::cout << "REMOVE Black PAWN" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Knight)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackKnights, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackKnights, squareSize, xOffset);
                                std::cout << "REMOVE BLACK Knight" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Bishop)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackBishops, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackBishops, squareSize, xOffset);
                                std::cout << "REMOVE BLACK BISHOP" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Rook)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackRooks, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackRooks, squareSize, xOffset);
                                std::cout << "REMOVE BLACK Rook" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::Queen)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackQueens, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackQueens, squareSize, xOffset);
                                std::cout << "REMOVE BLACK Queen" << std::endl;
                            }
                            else if (Board.Square[Helpers::toIndex(endRow, endCol)] == (Piece::Black | Piece::King)) {
                                LastPiece = Piece::returnAtPosition(endRow, endCol, BlackKings, squareSize, xOffset);
                                Board.removeAtPosition(endRow, endCol, BlackKings, squareSize, xOffset);
                                std::cout << "REMOVE BLACK King" << std::endl;
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

                            // Branch if Promotion Allowed
                            if (isPromotionAllowed) {
                                std::cout << "PAWN CAN PROMOTE" << std::endl;
                                isPromotion = true; // Toggle Promotion Menu
                                
                                // Branch For White or Black Promotion
                                if (isWhiteTurn) {
                                    Promotion.setPosition(xOffset + (endCol) * squareSize, 0 * squareSize); // Set Position of Promotion Menu
                                    ShowPromotion(5, endCol, isWhiteTurn, PromotionPieces, Textures, squareSize, xOffset); // Set Texture To Correct Colour
                                    promotionRow = endRow; // Allow Position Access Outside Of Scope
                                    promotionCol = endCol;
                                    Board.removeAtPosition(promotionRow, promotionCol, WhitePawns, squareSize, xOffset); // Remove Pawn Sprite
                                    
                                }
                                else {
                                    Promotion.setPosition(xOffset + (endCol)*squareSize, 4 * squareSize);
                                    ShowPromotion(3, endCol, isWhiteTurn, PromotionPieces, Textures, squareSize, xOffset);
                                    promotionRow = endRow;
                                    promotionCol = endCol;
                                    Board.removeAtPosition(promotionRow, promotionCol, BlackPawns, squareSize, xOffset);
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
                        Eval.convertToEvaluate(Board.Square); // Evaluate Board
                        Eval.evaluationBar(BlackBar); // Update Evaluation Bar
                        clock.restart();
                        currentTime = 0;

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

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                    std::cout << "ESCAPE PRESSED " << std::endl;
                    window.close();
                }

                if (event.key.scancode == sf::Keyboard::Scan::E) {
                    sf::Time elapsedTime = clock.getElapsedTime();
                    std::cout << "ELAPSED TIME: " << static_cast<int>(elapsedTime.asSeconds()) << std::endl;
                }

                if (event.key.scancode == sf::Keyboard::Scan::R) {
                    clock.restart();
                    std::cout << "CLOCK RESTARTED " << std::endl;
                }

                if (event.key.scancode == sf::Keyboard::Scan::D) {
                    std::cout << "CHECKMATE" << std::endl;
                }

            }
            
            
        }

        int elapsedTime = static_cast<int>(clock.getElapsedTime().asSeconds()); // Cast to Truncate to Int

        // Update Timer
        if ((elapsedTime > currentTime) && gameStart) { // If Elapsed Time Has Increased
            currentTime += 1; // Increment time
            if (isWhiteTurn) {
                WhiteTimeRemaining -= 1; // Decreasse Correct Time
            }
            else {
                BlackTimeRemaining -= 1;
            }
            // Update Text
            (isWhiteTurn) ? whiteTime.setString(int_to_Time(WhiteTimeRemaining)) : blackTime.setString(int_to_Time(BlackTimeRemaining));
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
        for (auto& queen : WhiteQueens) {
            window.draw(queen);
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

        for (auto& queen : BlackQueens) {
            window.draw(queen);
        }

        if (BlackKings.size()) {
            window.draw(BlackKings[0]);
        }

        window.draw(menuBar);
        window.draw(ChessIcon);
        window.draw(HomeIcon);
        window.draw(AnalysisIcon);
        //window.draw(playGame);

        

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
            window.draw(WhiteBar);
            window.draw(BlackBar);
        }
        
        if (playGameMode) {
            window.draw(WhiteTimeBox);
            window.draw(BlackTimeBox);
            window.draw(whiteTime);
            window.draw(blackTime);
        }
     
        
        //window.draw(playGameText);
        window.display();
    }
}

int main() {
    mainloop();
    return 0;
}