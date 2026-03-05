#include "CGs.h"

CGs::CGs(sf::RenderWindow& window) : window(window),font("openresources/uifont.ttf"),text(font) {}

void CGs::run(std::atomic<bool>& loaded){
    sf::Texture iconTex, logoTex;
    if (!iconTex.loadFromFile("openresources/icon.png") ||
        !logoTex.loadFromFile("openresources/logo.png"))
    {
        std::cout << "Image load failed\n";
        exit(-1);
    }

    sf::Sprite sprite(iconTex);
    sprite.setOrigin({iconTex.getSize().x / 2.f,
                     iconTex.getSize().y / 2.f});
    sprite.setPosition({(float)window.getPosition().x,(float)window.getPosition().y});

    // ===== 加载字体 =====
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Magenta);

    // ===== 加载音效 =====
    sf::Music music;
    if (!music.openFromFile("openresources/boot.mp3"))
    {
        std::cout << "Music load failed\n";
        exit(-1);
    }
    music.play();

    // ===== Shader =====
    sf::Shader shader;
    if (!shader.loadFromFile("openresources/boot.frag",
                             sf::Shader::Type::Fragment))
    {
        std::cout << "Shader load failed\n";
        exit(-1);
    }

    sf::Clock clock;

    while (window.isOpen()){
        if(loaded) return;

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
                exit(0);
            }
        }

        float time = clock.getElapsedTime().asSeconds();
        shader.setUniform("u_time", time);

        // ===== 时间轴切换 =====
        if (time < 3.0f){
            sprite.setTexture(iconTex, true);

            // 第一幕居中
            sprite.setOrigin({iconTex.getSize().x / 2.f,
                            iconTex.getSize().y / 2.f});
            sprite.setPosition({(float)window.getView().getSize().x/2, (float)window.getView().getSize().y/2});

            text.setString("Made With SFML");
            text.setPosition({(float)window.getView().getSize().x/1.5f, (float)window.getView().getSize().y/1.5f});
        }
        else {
            sprite.setTexture(logoTex, true);

            // 第二幕向左上移动
            sprite.setOrigin({logoTex.getSize().x / 2.f,
                            logoTex.getSize().y / 2.f});

            sprite.setPosition({(float)window.getView().getSize().x/2, (float)window.getView().getSize().y/2});

            text.setString(L"LBB 游戏");
            text.setPosition({(float)window.getView().getSize().x/1.5f, (float)window.getView().getSize().y/1.5f});
        }

        // ===== 冲击放大 =====
        float pulse = 0.7f + 0.1f * sin(time * 3.0f);
        sprite.setScale({pulse, pulse});

        window.clear(sf::Color::White);
        window.draw(sprite, &shader);
        window.draw(text);
        window.display();
    }
}