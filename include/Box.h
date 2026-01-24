#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Box {
    //Time
    float elapsedTime = 0.f;
    int currentFrame = 0;
    const float switchTime = 1.0f; // 每 1 秒切换一次

    sf::RectangleShape shape;
    static std::vector<sf::Texture> n_boxTextures;
    static std::vector<sf::Texture> t_boxTextures;
    sf::Sprite boxSprite;
    friend class Game;
    friend class Player;

public:
    Box(float x, float y, float width, float height,bool status);
    //void update(float dt);
};