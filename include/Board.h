#pragma once

#include "Map.h"

enum Move{
    Up,
    Down,
    Left,
    Right,
    None
};

static const std::vector<std::vector<int>> dir={{-1,0},{1,0},{0,-1},{0,1}};

class Board{
    void Init_player_position();

    bool isDeadCorner(int x,int y);

    friend class Trainer;
    friend class SokobanEnv;
    friend class LearningAgent;
    friend class Solution;
    friend class Game;

public:
    std::vector<std::string> current_map; 
    std::pair<int,int> player_position;

    Board();

    std::string MaptoString();

    std::vector<Move> generateMoves();
    bool canMove(Move m);
    Board applyMove(Move move);
    bool isTerminal();
    bool boxOnTargetIncreased(Board &b);
    int sumBoxToTargetDist();

    //Dead判断
    bool check2x2Dead(int y, int x);
    bool frozenOnWall(int y,int x);
    bool isDead();
};

