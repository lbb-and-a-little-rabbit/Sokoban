#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SaveSystem.h"

enum class MenuResult {
    None,
    StartGame,
    Exit,
    SelectLevel
};

class Menu {
private:
    sf::RenderWindow &window;

    static sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font font;
    sf::Text startText;
    sf::Text levelText;
    sf::Text exitText;

    void processEvents(MenuResult& result);
    void updateHover();
    void render();

    friend class Game;
    friend class LevelSelect;

public:
    int maxLevel=0;
    sf::Music currentMusic;

    Menu(sf::RenderWindow &window);
    MenuResult run();   // 运行菜单，返回结果
};
