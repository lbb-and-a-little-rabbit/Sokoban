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

    musicData=LoadFile("assets/temple.wav");
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

    float wx=window.getView().getSize().x;
    float wy=window.getView().getSize().y;

    // Start Game 文本
    startText.setString(L"新游戏");
    startText.setCharacterSize(48);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({wx/3.f, wy/2.f});

    // Level 文本
    levelText.setString(L"选择关卡");
    levelText.setCharacterSize(48);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition({wx/3.f, wy/2.f+80});

    // Exit 文本
    exitText.setString(L"退出");
    exitText.setCharacterSize(48);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({wx/3.f, wy/2.f+160});

    // 菜单音乐
    if (currentMusic.openFromMemory(musicData.data(),musicData.size())) {
        currentMusic.setLooping(true);
        currentMusic.play();
    }
    else {
        std::cerr << "Music loaded failed!";
        exit(-1);
    }

    // 存档信息
    SaveData save = SaveSystem::load();
    maxLevel = save.maxUnlockedLevel;

    //Shader
    if (!shader.loadFromFile("openresources/menu.frag", sf::Shader::Type::Fragment)){
        std::cerr << "Shader load failed\n";
        exit(-1);
    }
    screen.setSize(window.getView().getSize());
}

// ===== 菜单主循环 =====
MenuResult Menu::run() {
    MenuResult result = MenuResult::None;

    sf::Clock clock;
    while (window.isOpen() && result == MenuResult::None) {
        processEvents(result,clock);
        updateHover();
        render();
    }

    return result;
}

// ===== 事件处理 =====
void Menu::processEvents(MenuResult& result,sf::Clock& clock) {
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
    float time = clock.getElapsedTime().asSeconds();

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouse = window.mapPixelToCoords(mousePixel);

    shader.setUniform("time", time);
    shader.setUniform("resolution", window.getView().getSize());
    shader.setUniform("mouse", mouse);
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
    window.draw(screen,&shader);
    window.display();
}