#include "Player.h"

std::vector<sf::Texture> Player::playerTextures;

void Player::LoadTextures(){
    if(!playerTextures.empty()) return;

    const char* files[] = {
        "assets/player_02.png",
        "assets/player_01.png",
        "assets/player_03.png",
        "assets/player_04.png"
    };

    for(auto f:files){
        auto data = LoadFile(f);

        sf::Texture tex;
        if(!tex.loadFromMemory(data.data(),data.size())){
            std::cerr<<"Failed to load "<<f<<"\n";
            exit(-1);
        }

        playerTextures.push_back(std::move(tex));
    }
}

Player::Player(float x, float y, float width, float height,PlayerForward pf) : playerSprite(playerTextures[(int)pf]),pf(pf) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    playerSprite.setPosition({x, y});
    sf::Vector2u textureSize = playerTextures[(int)pf].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    playerSprite.setScale(scale);
}