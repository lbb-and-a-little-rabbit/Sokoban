#include "LevelSelect.h"
#include <iostream>

// ===== 静态成员定义 =====
sf::Texture LevelSelect::backgroundTexture;
std::vector<char> LevelSelect::fontData;
sf::Font LevelSelect::font;

void LevelSelect::LoadTextures() {
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
LevelSelect::LevelSelect(sf::RenderWindow &window,int maxLevel)
: window(window),backgroundSprite(backgroundTexture),maxLevel(maxLevel),MenuText(font)
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

    // Menu 文本
    MenuText.setString(L"返回菜单");
    MenuText.setCharacterSize(48);
    MenuText.setFillColor(sf::Color::White);
    MenuText.setPosition({wx/2.f, wy/8.f});

    // Level 文本
    float startX = wx/4.f;
    float startY = wy/2.f;
    float gapX = 100.f;
    float gapY = 40.f;

    for(int i = 0; i <= maxLevel; i++){
        int col = i / 5;  // 第几列
        int row = i % 5;  // 第几行

        sf::Text text(font);
        text.setString(L"关卡 " + std::to_wstring(i));
        text.setPosition({startX + col * gapX, startY + row * gapY});
        levelTexts.push_back(text);
    }
}

// ===== 菜单主循环 =====
std::pair<LevelResult,int> LevelSelect::run() {
    LevelResult result = LevelResult::None;

    while (window.isOpen() && result == LevelResult::None) {
        processEvents(result);
        updateHover();
        render();
    }

    return {result,unit};
}

// ===== 事件处理 =====
void LevelSelect::processEvents(LevelResult& result) {
    while (auto event = window.pollEvent()) {

        // 关闭窗口
        if (event->is<sf::Event::Closed>()) {
            result = LevelResult::Close;
            return; // 已决定退出，可以直接返回
        }

        // 鼠标按下事件
        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                // 转换为世界坐标
                sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);

                // 检查是否点击了 Level 文本
                for(int i=0;i<=maxLevel;i++){
                    sf::Text &t=levelTexts[i];
                    if(t.getGlobalBounds().contains(mousePos)){
                        result=LevelResult::Selected;
                        unit=i;
                        return;
                    }
                }

                // Menu
                if(MenuText.getGlobalBounds().contains(mousePos)){
                    result=LevelResult::Exit;
                    return;
                }
            }
        }
    }
}

// ===== 悬停高亮 =====
void LevelSelect::updateHover() {
    sf::Vector2f mouse =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for(sf::Text &t:levelTexts){
        if (t.getGlobalBounds().contains(mouse)) {
            t.setFillColor(sf::Color::Red);
        } else {
            t.setFillColor(sf::Color::White);
        }
    }

    if (MenuText.getGlobalBounds().contains(mouse)) {
        MenuText.setFillColor(sf::Color::Red);
    } else {
        MenuText.setFillColor(sf::Color::White);
    }
}

// ===== 渲染 =====
void LevelSelect::render() {
    window.clear();
    window.draw(backgroundSprite);
    for(sf::Text &t:levelTexts) window.draw(t);
    window.draw(MenuText);
    window.display();
}