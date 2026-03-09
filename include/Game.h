#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Map.h"
#include "Board.h"

#include "SaveSystem.h"
#include "Solution.h"

#include "Wall.h"
#include "Box.h"
#include "Target.h"
#include "Player.h"
#include "Ground.h"

#include "Trainer.h"

class Game{
    unsigned int Window_Width;
    unsigned int Window_Height;
    sf::RenderWindow &window;

    Map maps_assistant;
    int current_level;
    Board current_board;
    bool islevelCompleted=false;

    std::vector<Ground> grounds;
    std::vector<Wall> walls;
    std::vector<Target> targets;
    std::vector<Box> boxes;
    Player* player;

    // 自动求解相关
    bool isautoSolving = false;
    bool autoSolving = false;
    std::vector<Move> solutionMoves;
    size_t solutionIndex = 0;
    sf::Clock autoSolveClock;
    float autoSolveInterval = 0.3f;

    //music
    static std::vector<char> bgmData;
    sf::Music music;
    static sf::SoundBuffer walkbuffer;
    sf::Sound walksound;
    static std::vector<char> level_comData;
    sf::Music level_complete;

    //Text
    static std::vector<char> fontData;
    sf::Font uiFont;
    std::vector<sf::Text> helpTexts;
    bool showHelp = true;

    //Esc
    bool backtomenu=false;

    void Clear();
    void Set_based_on_board(Move move);

    void processEvents();
    void update();
    void render();

public:
    Game(sf::RenderWindow &window,unsigned int w=800,unsigned int h=600,int cur_level=0);
    static void LoadTextures(); // 在 PhysFS 挂载后调用一次

    void run();
};