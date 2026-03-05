#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <atomic>
#include <thread>

#include "physfs_assistant.h"

class CGs{
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text text;

public:
    CGs(sf::RenderWindow& window);
    void run(std::atomic<bool>& loaded);
};