#include "Wall.h"

sf::Texture wall1("assets/wall.png");

std::vector<sf::Texture> Wall::wallTextures;

Wall::Wall(float x, float y, float width, float height) : wallSprite(wall1) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    if(wallTextures.size()<1){
        wallTextures.push_back(wall1);
    }

    wallSprite.setPosition({x, y});
    sf::Vector2u textureSize = wallTextures[0].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    wallSprite.setScale(scale);
}