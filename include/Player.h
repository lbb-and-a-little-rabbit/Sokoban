#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "physfs_assistant.h"

class Player{
    //Time
    float elapsedTime = 0.f;
    int currentFrame = 0;
    const float switchTime = 1.0f; // 每 1 秒切换一次

    sf::RectangleShape shape;
    static std::vector<sf::Texture> playerTextures;
    sf::Sprite playerSprite; 
    friend class Game;

public:
    Player(float x, float y, float width, float height); 
    static void LoadTextures(); // 在 PhysFS 挂载后调用一次
};