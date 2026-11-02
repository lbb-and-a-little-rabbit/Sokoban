#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SaveSystem.h"
#include "physfs_assistant.h"

enum class LevelResult { None, Selected, Exit, Close };

class LevelSelect {
private:
    sf::RenderWindow& window;
    static sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    static std::vector<char> fontData;
    static sf::Font font;
    sf::Text MenuText, heading, pageText, previousText, nextText;
    sf::RectangleShape panel, previousButton, nextButton;
    std::vector<sf::Text> levelTexts;
    std::vector<sf::RectangleShape> levelCards;
    static constexpr int levelsPerPage = 12;
    int maxLevel = 0;
    int unit = 0;
    int currentPage = 0;
    int levelCount = 0;
    int pageCount = 1;
    void rebuildPage();
    void changePage(int delta);
    void processEvents(LevelResult& result);
    void updateHover();
    void render();
public:
    LevelSelect(sf::RenderWindow& window, int maxLevel);
    std::pair<LevelResult,int> run();
    static void LoadTextures();
};
