#include "Player.h"

std::vector<sf::Texture> Player::playerTextures;

void Player::LoadTextures(){
    if(playerTextures.empty()){
        auto data=LoadFile("assets/player.png");
        sf::Texture player1;
        if(!player1.loadFromMemory(data.data(),data.size())){
            std::cerr << "Failed to load!";
            exit(-1);
        }
        playerTextures.push_back(std::move(player1));
    }
}

Player::Player(float x, float y, float width, float height) : playerSprite(playerTextures[0]) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    playerSprite.setPosition({x, y});
    sf::Vector2u textureSize = playerTextures[0].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    playerSprite.setScale(scale);
}