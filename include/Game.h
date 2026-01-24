#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "Map.h"
#include "Board.h"

#include "Solution.h"

#include "Wall.h"
#include "Box.h"
#include "Target.h"
#include "Player.h"

class Game{
    unsigned int Window_Width;
    unsigned int Window_Height;
    sf::RenderWindow window;

    Map maps_assistant;
    int current_level;
    Board current_board;

    std::vector<Wall> walls;
    std::vector<Target> targets;
    std::vector<Box> boxes;
    Player* player;

    void Clear();
    void Set_based_on_board();

    void processEvents();
    void update();
    void render();

public:
    Game(unsigned int w=800,unsigned int h=600,int cur_level=0);

    void run();
};