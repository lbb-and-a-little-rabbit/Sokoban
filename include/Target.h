#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "physfs_assistant.h"

class Target {
    //Time
    float elapsedTime = 0.f;
    int currentFrame = 0;
    const float switchTime = 1.0f; // 每 1 秒切换一次

    sf::RectangleShape shape;
    static std::vector<sf::Texture> targetTextures;
    sf::Sprite targetSprite;
    friend class Game;
    friend class Player;

public:
    Target(float x, float y, float width, float height);
    static void LoadTextures(); // 在 PhysFS 挂载后调用一次
    //void update(float dt);
};