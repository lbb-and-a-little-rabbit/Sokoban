#pragma once

#include "Board.h"

class Agent{
public:
    virtual ~Agent()=default;

    virtual Move act(Board& state)=0;

    virtual void observe(Board& s,
                         Move a,
                         int reward,
                         Board& s2,
                         bool done) {}
};