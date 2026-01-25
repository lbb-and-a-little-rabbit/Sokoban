#pragma once

#include <chrono>
#include <algorithm>
#include <unordered_map>
#include <queue>

#include "Board.h"

constexpr double TIME_LIMIT = 5.0;

struct Result{
    bool timeout;
    bool solvable;
    int best_steps;
    std::vector<Move> path;

    Result() : timeout(false),solvable(false),best_steps(-1) {};
};

class Solution{
    Board current_board;

    Result UCS();
    Result Astar();

public:
    Solution(int idx,Map &m);
    Solution(std::vector<std::string> cur_map);
    Solution(Board board);

    Result solve(int choice);
};