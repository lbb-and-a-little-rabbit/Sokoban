#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SaveSystem.h"
#include "physfs_assistant.h"

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

    static std::vector<char> fontData; // 存储内存
    static sf::Font font;
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
    static void LoadTextures(); // 在 PhysFS 挂载后调用一次
};
