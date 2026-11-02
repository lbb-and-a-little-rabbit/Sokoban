#include "DisplayScaling.h"
#include "LevelSelect.h"
#include "Map.h"
#include <algorithm>

sf::Texture LevelSelect::backgroundTexture;
std::vector<char> LevelSelect::fontData;
sf::Font LevelSelect::font;

namespace {
void centerText(sf::Text& text, sf::Vector2f center) {
    const auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);
    text.setPosition(center);
}
}

void LevelSelect::LoadTextures() {
    auto data = LoadFile("assets/menu_background.png");
    if(!backgroundTexture.loadFromMemory(data.data(), data.size()))
        throw std::runtime_error("Cannot load level selection background");
    fontData = LoadFile("assets/uifont.ttf");
    if(!font.openFromMemory(fontData.data(), fontData.size()))
        throw std::runtime_error("Cannot load level selection font");
}

LevelSelect::LevelSelect(sf::RenderWindow& window, int unlocked)
: window(window), backgroundSprite(backgroundTexture), MenuText(font), heading(font),
  pageText(font), previousText(font), nextText(font) {
    levelCount = Map().getLevelCount();
    maxLevel = std::clamp(unlocked, 0, levelCount - 1);
    pageCount = (levelCount + levelsPerPage - 1) / levelsPerPage;
    currentPage = maxLevel / levelsPerPage;
    rebuildPage();
}

void LevelSelect::rebuildPage() {
    const auto size = window.getView().getSize();
    const auto origin = window.getView().getCenter() - size / 2.f;
    auto point = [&](float x, float y) { return origin + sf::Vector2f(size.x*x, size.y*y); };
    const auto textureSize = backgroundTexture.getSize();
    backgroundSprite.setPosition(origin);
    backgroundSprite.setScale({size.x/textureSize.x, size.y/textureSize.y});
    // The background title occupies the upper-left 40% of the image.
    panel.setPosition(point(.22f, .44f));
    panel.setSize({size.x*.72f, size.y*.51f});
    panel.setFillColor(sf::Color(18, 29, 42, 235));
    panel.setOutlineColor(sf::Color(105, 132, 151, 160));
    panel.setOutlineThickness(1.f);
    const unsigned textSize = static_cast<unsigned>(std::max(16.f, size.y*.023f));
    MenuText.setString(L"Esc  返回菜单");
    MenuText.setCharacterSize(textSize);
    centerText(MenuText, point(.83f, .49f));
    heading.setString(L"选择关卡");
    heading.setCharacterSize(textSize + 6);
    heading.setPosition(point(.25f, .47f));
    levelTexts.clear();
    levelCards.clear();
    const int begin = currentPage * levelsPerPage;
    for(int i = begin; i < std::min(begin + levelsPerPage, levelCount); ++i) {
        const int slot = i - begin;
        sf::RectangleShape card({size.x*.15f, size.y*.073f});
        card.setPosition(point(.25f + (slot%4)*.165f, .55f + (slot/4)*.095f));
        card.setOutlineThickness(1.f);
        levelCards.push_back(card);
        sf::Text text(font);
        text.setCharacterSize(textSize);
        text.setString(L"关卡 " + std::to_wstring(i) + (i > maxLevel ? L" · 锁定" : L""));
        centerText(text, card.getPosition() + card.getSize()/2.f);
        levelTexts.push_back(text);
    }
    previousButton.setSize({size.x*.14f, size.y*.055f});
    previousButton.setPosition(point(.25f, .865f));
    nextButton.setSize(previousButton.getSize());
    nextButton.setPosition(point(.77f, .865f));
    previousText.setString(L"← 上一页");
    nextText.setString(L"下一页 →");
    previousText.setCharacterSize(textSize);
    nextText.setCharacterSize(textSize);
    centerText(previousText, previousButton.getPosition()+previousButton.getSize()/2.f);
    centerText(nextText, nextButton.getPosition()+nextButton.getSize()/2.f);
    pageText.setCharacterSize(textSize);
    pageText.setString(std::to_wstring(currentPage+1) + L" / " + std::to_wstring(pageCount)
        + L" 页 · 共 " + std::to_wstring(levelCount) + L" 关");
    centerText(pageText, point(.58f, .8925f));
}

void LevelSelect::changePage(int delta) {
    const int next = std::clamp(currentPage+delta, 0, pageCount-1);
    if(next != currentPage) { currentPage = next; rebuildPage(); }
}

std::pair<LevelResult,int> LevelSelect::run() {
    LevelResult result = LevelResult::None;
    while(window.isOpen() && result == LevelResult::None) {
        processEvents(result);
        if(result != LevelResult::None) break;
        updateHover();
        render();
        sf::sleep(sf::milliseconds(10));
    }
    return {result, unit};
}

void LevelSelect::processEvents(LevelResult& result) {
    while(auto event = window.pollEvent()) {
            if(DisplayScaling::handleEvent(window, *event)) continue;
        if(event->is<sf::Event::Closed>()) { result = LevelResult::Close; return; }
        if(const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if(key->scancode == sf::Keyboard::Scancode::Escape) { result = LevelResult::Exit; return; }
            if(key->scancode == sf::Keyboard::Scancode::Left || key->scancode == sf::Keyboard::Scancode::PageUp) changePage(-1);
            if(key->scancode == sf::Keyboard::Scancode::Right || key->scancode == sf::Keyboard::Scancode::PageDown) changePage(1);
        }
        if(const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if(mouse->button != sf::Mouse::Button::Left) continue;
            const auto pos = window.mapPixelToCoords(mouse->position);
            if(MenuText.getGlobalBounds().contains(pos)) { result = LevelResult::Exit; return; }
            if(previousButton.getGlobalBounds().contains(pos)) { changePage(-1); continue; }
            if(nextButton.getGlobalBounds().contains(pos)) { changePage(1); continue; }
            for(size_t i = 0; i < levelCards.size(); ++i) {
                const int level = currentPage*levelsPerPage + static_cast<int>(i);
                if(level <= maxLevel && levelCards[i].getGlobalBounds().contains(pos)) {
                    unit = level;
                    result = LevelResult::Selected;
                    return;
                }
            }
        }
    }
}

void LevelSelect::updateHover() {
    const auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for(size_t i = 0; i < levelCards.size(); ++i) {
        const bool unlocked = currentPage*levelsPerPage + static_cast<int>(i) <= maxLevel;
        const bool hover = unlocked && levelCards[i].getGlobalBounds().contains(mouse);
        levelCards[i].setFillColor(!unlocked ? sf::Color(33, 43, 54) :
            (hover ? sf::Color(77, 105, 122) : sf::Color(45, 66, 83)));
        levelCards[i].setOutlineColor(hover ? sf::Color(255, 209, 104) : sf::Color(70, 89, 104));
        levelTexts[i].setFillColor(unlocked ? sf::Color(245, 240, 225) : sf::Color(137, 148, 158));
    }
    auto styleButton = [&](sf::RectangleShape& button, sf::Text& label, bool enabled) {
        button.setFillColor(enabled && button.getGlobalBounds().contains(mouse) ?
            sf::Color(77, 105, 122) : sf::Color(33, 48, 63));
        label.setFillColor(enabled ? sf::Color(245, 240, 225) : sf::Color(100, 115, 126));
    };
    styleButton(previousButton, previousText, currentPage > 0);
    styleButton(nextButton, nextText, currentPage+1 < pageCount);
    MenuText.setFillColor(MenuText.getGlobalBounds().contains(mouse) ? sf::Color(255,209,104) : sf::Color::White);
}

void LevelSelect::render() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(panel);
    window.draw(heading);
    window.draw(MenuText);
    for(size_t i = 0; i < levelCards.size(); ++i) {
        window.draw(levelCards[i]);
        window.draw(levelTexts[i]);
    }
    window.draw(previousButton); window.draw(previousText);
    window.draw(nextButton); window.draw(nextText);
    window.draw(pageText);
    window.display();
}
