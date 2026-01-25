#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SaveSystem.h"

enum class LevelResult{
    None,
    Selected,
    Exit,
    Close
};


class LevelSelect {
private:
    sf::RenderWindow &window;

    static sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font font;
    sf::Text MenuText;
    std::vector<sf::Text> levelTexts;

    int maxLevel=0;
    int unit=0;

    void processEvents(LevelResult &result);
    void updateHover();
    void render();

public:
    LevelSelect(sf::RenderWindow &window,int maxLevel);
    std::pair<LevelResult,int> run();   // 运行菜单，返回结果
};
