#pragma once

#include "Board.h"

struct Result{
    bool solvable;
    int best_steps;
    std::vector<char> res;

    Result() : solvable(false),best_steps(-1) {};
};

class Solution{
    Board current_board;

    Result UCS();

public:
    Solution(int idx,Map &m);
    Solution(std::vector<std::string> cur_map);

    Result solve(int choice);
};