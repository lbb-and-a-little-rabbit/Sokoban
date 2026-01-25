#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class MenuResult {
    None,
    StartGame,
    Exit
};

class Menu {
private:
    sf::RenderWindow &window;

    static sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font font;
    sf::Text startText;
    sf::Text exitText;

    sf::Music currentMusic;

    void processEvents(MenuResult& result);
    void updateHover();
    void render();

public:
    Menu(sf::RenderWindow &window);
    MenuResult run();   // 运行菜单，返回结果
};
