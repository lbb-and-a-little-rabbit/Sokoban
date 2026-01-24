#include "Player.h"

sf::Texture player1("assets/player.png");

std::vector<sf::Texture> Player::playerTextures;

Player::Player(float x, float y, float width, float height) : playerSprite(player1) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    if(playerTextures.size()<1){
        playerTextures.push_back(player1);
    }

    playerSprite.setPosition({x, y});
    sf::Vector2u textureSize = playerTextures[0].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    playerSprite.setScale(scale);
}