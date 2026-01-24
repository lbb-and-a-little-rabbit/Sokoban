#include "Target.h"

sf::Texture target1("assets/target.jpg");

std::vector<sf::Texture> Target::targetTextures;

Target::Target(float x, float y, float width, float height) : targetSprite(target1) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    if(targetTextures.size()<1){
        targetTextures.push_back(target1);
    }

    targetSprite.setPosition({x, y});
    sf::Vector2u textureSize = targetTextures[0].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    targetSprite.setScale(scale);
}