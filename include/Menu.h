#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SaveSystem.h"
#include "physfs_assistant.h"
#include "EncryptedPak.h"

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

    static std::vector<char> fontData; // 存储内存
    static std::vector<char> musicData;
    static sf::Font font;
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
    static void LoadTextures(); // 在 PhysFS 挂载后调用一次
};
