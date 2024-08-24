#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Helpers.h"


namespace Piece {
    const int None = 0; // E.g. Piece::Black | Piece:: Knight = 10
    const int Pawn = 1;
    const int Knight = 2;
    const int Bishop = 3;
    const int Rook = 4;
    const int Queen = 5;
    const int King = 6;
    const int White = 0;
    const int Black = 8;
}