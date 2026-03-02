#include "Wall.h"

std::vector<sf::Texture> Wall::wallTextures;

void Wall::LoadTextures(){
    if(wallTextures.empty()){
        auto data=LoadFile("assets/wall.png");
        sf::Texture wall1;
        if(!wall1.loadFromMemory(data.data(),data.size())){
            std::cerr << "Failed to load!";
            exit(-1);
        }
        wallTextures.push_back(std::move(wall1));
    }
}

Wall::Wall(float x, float y, float width, float height) : wallSprite(wallTextures[0]) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    wallSprite.setPosition({x, y});
    sf::Vector2u textureSize = wallTextures[0].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    wallSprite.setScale(scale);
}