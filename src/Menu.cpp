#include "Menu.h"
#include <iostream>

// ===== 静态成员定义 =====
sf::Texture Menu::backgroundTexture;
std::vector<char> Menu::fontData;
std::vector<char> Menu::musicData;
sf::Font Menu::font;

void Menu::LoadTextures(){
    auto data=LoadFile("assets/menu_background.png");
    if(!backgroundTexture.loadFromMemory(data.data(),data.size())){
        std::cerr << "Failed to load!";
        exit(-1);
    }

    fontData=LoadFile("assets/uifont.ttf");
    if(!font.openFromMemory(fontData.data(),fontData.size())){
        std::cerr << "Failed to load!";
        exit(-1);
    }
}

// ===== 构造函数 =====
Menu::Menu(sf::RenderWindow &window)
: window(window),backgroundSprite(backgroundTexture),startText(font),levelText(font),exitText(font)
{
    // 缩放铺满窗口
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2f windowSize = window.getView().getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    backgroundSprite.setScale(scale);

    // Start Game 文本
    startText.setString(L"新游戏");
    startText.setCharacterSize(48);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({300.f, 250.f});

    // Level 文本
    levelText.setString(L"选择关卡");
    levelText.setCharacterSize(48);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition({300.f, 330.f});

    // Exit 文本
    exitText.setString(L"退出");
    exitText.setCharacterSize(48);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({300.f, 410.f});

    // 菜单音乐
    auto t1 = std::chrono::high_resolution_clock::now();

    std::string path = "temp/temple.wav";

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Decrypt time: "
    << std::chrono::duration<double>(t2 - t1).count()
    << "s\n";

    if (currentMusic.openFromFile(path)) {
        currentMusic.setLooping(true);
        currentMusic.play();
    }

    // 存档信息
    SaveData save = SaveSystem::load();
    maxLevel = save.maxUnlockedLevel;
}

// ===== 菜单主循环 =====
MenuResult Menu::run() {
    MenuResult result = MenuResult::None;

    while (window.isOpen() && result == MenuResult::None) {
        processEvents(result);
        updateHover();
        render();
    }

    return result;
}

// ===== 事件处理 =====
void Menu::processEvents(MenuResult& result) {
    while (auto event = window.pollEvent()) {

        // 关闭窗口
        if (event->is<sf::Event::Closed>()) {
            result = MenuResult::Exit;
            return; // 已决定退出，可以直接返回
        }

        // 鼠标按下事件
        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                // 转换为世界坐标
                sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);

                // 检查是否点击了 Start Level Exit 文本
                if (startText.getGlobalBounds().contains(mousePos)) {
                    result = MenuResult::StartGame;
                    currentMusic.stop();
                    return; // 点击后立即返回
                }

                if (levelText.getGlobalBounds().contains(mousePos)) {
                    result = MenuResult::SelectLevel;
                    return;
                }

                if (exitText.getGlobalBounds().contains(mousePos)) {
                    result = MenuResult::Exit;
                    return;
                }
            }
        }
    }
}

// ===== 悬停高亮 =====
void Menu::updateHover() {
    sf::Vector2f mouse =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (startText.getGlobalBounds().contains(mouse)) {
        startText.setFillColor(sf::Color::Yellow);
    } else {
        startText.setFillColor(sf::Color::White);
    }

    if (levelText.getGlobalBounds().contains(mouse)) {
        levelText.setFillColor(sf::Color::Magenta);
    } else {
        levelText.setFillColor(sf::Color::White);
    }

    if (exitText.getGlobalBounds().contains(mouse)) {
        exitText.setFillColor(sf::Color::Red);
    } else {
        exitText.setFillColor(sf::Color::White);
    }
}

// ===== 渲染 =====
void Menu::render() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(startText);
    window.draw(levelText);
    window.draw(exitText);
    window.display();
}