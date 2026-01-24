#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Wall {
    //Time
    float elapsedTime = 0.f;
    int currentFrame = 0;
    const float switchTime = 1.0f; // 每 1 秒切换一次

    sf::RectangleShape shape;
    static std::vector<sf::Texture> wallTextures;
    sf::Sprite wallSprite;
    friend class Game;
    friend class Player;

public:
    Wall(float x, float y, float width, float height);
    //void update(float dt);
};