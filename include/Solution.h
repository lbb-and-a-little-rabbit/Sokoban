#pragma once

#include <algorithm>
#include <unordered_map>
#include <queue>

#include "Board.h"

struct Result{
    bool solvable;
    int best_steps;
    std::vector<Move> path;

    Result() : solvable(false),best_steps(-1) {};
};

class Solution{
    Board current_board;

    Result UCS();

public:
    Solution(int idx,Map &m);
    Solution(std::vector<std::string> cur_map);
    Solution(Board board);

    Result solve(int choice);
};