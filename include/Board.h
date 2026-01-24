#pragma once

#include "Map.h"

enum Move{
    Up,
    Down,
    Left,
    Right
};

static const std::vector<std::vector<int>> dir={{-1,0},{1,0},{0,-1},{0,1}};

class Board{
    std::vector<std::string> current_map; 
    std::pair<int,int> player_position;

    void Init_player_position();

    friend class Solution;
    friend class Game;

public:
    Board();

    std::vector<Move> generateMoves();
    Board applyMove(Move move);
};

