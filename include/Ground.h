#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "physfs_assistant.h"

class Ground{
    sf::RectangleShape shape;
    static std::vector<sf::Texture> groundTextures;
    sf::Sprite groundSprite;
    friend class Game;
    friend class Player;

public:
    Ground(float x, float y, float width, float height);
    static void LoadTextures(); // 在 PhysFS 挂载后调用一次
};