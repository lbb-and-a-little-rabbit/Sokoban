#pragma once

#include "Board.h"

struct StepResult{
    Board next;
    int reward;
    bool done;
};

class SokobanEnv{
    Board cur;

public:
    SokobanEnv(Board& b);

    Board& getState();
    bool isDone();
    StepResult step(Move m);
    void reset(Board& re);
};