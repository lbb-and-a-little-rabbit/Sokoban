#include "Target.h"

std::vector<sf::Texture> Target::targetTextures;

void Target::LoadTextures(){
    if(targetTextures.empty()){
        auto data=LoadFile("assets/target.jpg");
        sf::Texture tar1;
        if(!tar1.loadFromMemory(data.data(),data.size())){
            std::cerr << "Failed to load!";
            exit(-1);
        }
        targetTextures.push_back(std::move(tar1));
    }
}

Target::Target(float x, float y, float width, float height) : targetSprite(targetTextures[0]) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    targetSprite.setPosition({x, y});
    sf::Vector2u textureSize = targetTextures[0].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    targetSprite.setScale(scale);
}