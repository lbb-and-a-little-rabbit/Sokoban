#include "DisplayScaling.h"
#include "CGs.h"
#include <algorithm>

sf::Font CGs::font;
std::vector<char> CGs::fontdata;
std::vector<char> CGs::icondata;
std::vector<char> CGs::logodata;
std::vector<char> CGs::musicdata;

void CGs::LoadTextures(){
    fontdata = LoadFile("assets/uifont.ttf");
    if(!font.openFromMemory(fontdata.data(),fontdata.size()))
        throw std::runtime_error("Cannot load intro font");
    icondata = LoadFile("assets/icon.png");
    logodata = LoadFile("assets/logo.png");
    musicdata = LoadFile("assets/boot.mp3");
}

CGs::CGs(sf::RenderWindow& window) : window(window),text(font) {}

void CGs::run(std::atomic<bool>& loaded){
    sf::Texture iconTex, logoTex;
    if(!iconTex.loadFromMemory(icondata.data(),icondata.size()) ||
       !logoTex.loadFromMemory(logodata.data(),logodata.size()))
        throw std::runtime_error("Cannot load intro images");
    iconTex.setSmooth(true);
    logoTex.setSmooth(true);
    sf::Music music;
    const bool hasMusic = music.openFromMemory(musicdata.data(),musicdata.size());
    if(hasMusic) { music.setVolume(0.f); music.play(); }
    sf::Clock clock;
    bool skipRequested = false;
    float exitStarted = -1.f;
    auto smooth = [](float t) { t=std::clamp(t,0.f,1.f); return t*t*(3.f-2.f*t); };
    while(window.isOpen()) {
        while(const auto event = window.pollEvent()) {
            if(DisplayScaling::handleEvent(window, *event)) continue;
            if(event->is<sf::Event::Closed>()) { window.close(); return; }
            if(const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if(key->scancode == sf::Keyboard::Scancode::Enter || key->scancode == sf::Keyboard::Scancode::Space ||
                   key->scancode == sf::Keyboard::Scancode::Escape) skipRequested = true;
            }
        }
        const float time = clock.getElapsedTime().asSeconds();
        const bool ready = loaded.load();
        if(ready && (skipRequested || time >= 4.8f) && exitStarted < 0.f) exitStarted = time;
        const float fadeOut = exitStarted < 0.f ? 0.f : smooth((time-exitStarted)/.45f);
        if(exitStarted >= 0.f && time-exitStarted >= .45f) return;
        if(hasMusic) music.setVolume(65.f*smooth(time/.7f)*(1.f-fadeOut));
        const auto size = window.getView().getSize();
        const auto center = window.getView().getCenter();
        const auto origin = center - size/2.f;
        const float unit = std::min(size.x, size.y);
        window.clear(sf::Color(17, 24, 35));
        // Quiet geometric background; no shader required on older GPUs.
        for(int i=0; i<5; ++i) {
            const float side=unit*(.18f+i*.07f);
            sf::RectangleShape outline({side,side});
            outline.setOrigin({side/2.f,side/2.f});
            outline.setPosition(center);
            outline.setRotation(sf::degrees(45.f+time*1.5f+i*7.f));
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(sf::Color(64,85,106,35));
            window.draw(outline);
        }
        auto drawScene = [&](const sf::Texture& texture, const std::wstring& title,
                             const std::wstring& caption, float opacity, float age) {
            if(opacity <= 0.f) return;
            const auto alpha = static_cast<std::uint8_t>(255.f*std::clamp(opacity,0.f,1.f));
            sf::Sprite sprite(texture);
            const auto dimensions = texture.getSize();
            sprite.setOrigin({dimensions.x/2.f,dimensions.y/2.f});
            const float scale=unit*.32f/std::max(dimensions.x,dimensions.y)*(.94f+.06f*smooth(age/1.5f));
            sprite.setScale({scale,scale});
            sprite.setPosition(center + sf::Vector2f(0.f,-unit*.07f+12.f*(1.f-smooth(age/.7f))));
            sprite.setColor(sf::Color(255,255,255,alpha));
            window.draw(sprite);
            auto label = [&](const std::wstring& value, float y, unsigned fontSize, sf::Color color) {
                text.setString(value);
                text.setCharacterSize(fontSize);
                color.a=alpha;
                text.setFillColor(color);
                const auto bounds=text.getLocalBounds();
                text.setOrigin(bounds.position+bounds.size/2.f);
                text.setPosition(center+sf::Vector2f(0.f,unit*y));
                window.draw(text);
            };
            label(title,.17f,static_cast<unsigned>(unit*.035f),sf::Color(242,238,229));
            label(caption,.225f,static_cast<unsigned>(unit*.018f),sf::Color(156,174,190));
        };
        const float firstOpacity=smooth(time/.65f)*(1.f-smooth((time-1.9f)/.5f));
        const float secondOpacity=smooth((time-2.4f)/.65f);
        drawScene(iconTex,L"Made with SFML",L"图形 · 音频 · 交互",firstOpacity,time);
        drawScene(logoTex,L"LBB 游戏",L"推箱子  /  SOKOBAN",secondOpacity,time-2.4f);
        sf::Text status(font, ready ? L"Enter / Space 跳过" :
            (skipRequested ? L"正在完成资源加载，请稍候…" : L"正在加载资源…"),
            static_cast<unsigned>(std::max(16.f,unit*.018f)));
        const auto bounds=status.getLocalBounds();
        status.setOrigin(bounds.position+bounds.size/2.f);
        status.setPosition({center.x,origin.y+size.y*.89f});
        status.setFillColor(sf::Color(156,174,190));
        window.draw(status);
        if(!ready) {
            const float width=unit*.24f;
            sf::RectangleShape track({width,3.f});
            track.setPosition({center.x-width/2.f,origin.y+size.y*.85f});
            track.setFillColor(sf::Color(48,63,78));
            window.draw(track);
            sf::RectangleShape segment({width*.25f,3.f});
            segment.setPosition(track.getPosition()+sf::Vector2f(width*.75f*(.5f+.5f*std::sin(time*2.f)),0.f));
            segment.setFillColor(sf::Color(211,184,116));
            window.draw(segment);
        }
        if(fadeOut > 0.f) {
            sf::RectangleShape curtain(size);
            curtain.setPosition(origin);
            curtain.setFillColor(sf::Color(17,24,35,static_cast<std::uint8_t>(fadeOut*255.f)));
            window.draw(curtain);
        }
        window.display();
        sf::sleep(sf::milliseconds(16));
    }
}
