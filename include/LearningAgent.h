#pragma once

#include "Agent.h"
#include <unordered_map>
#include <array>
#include <algorithm>
#include <fstream>

class LearningAgent : public Agent {
public:
    Move act(Board& state) override;
    void observe(Board& s,
                 Move a,
                 int reward,
                 Board& s2,
                 bool done) override;

    void decayEpsilon();
    void save(const std::string& path);

private:
    double alpha = 0.1;   // 学习率
    double gamma = 0.95;  // 折扣因子
    double epsilon = 0.1; // 探索率

    // Q[state][action]
    std::unordered_map<std::string,
        std::array<double,4>> Q;
};