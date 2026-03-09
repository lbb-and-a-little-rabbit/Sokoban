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

        data=LoadFile("assets/wall2.png");
        sf::Texture wall2;
        if(!wall2.loadFromMemory(data.data(),data.size())){
            std::cerr << "Failed to load!";
            exit(-1);
        }
        wallTextures.push_back(std::move(wall2));
    }
}

Wall::Wall(float x, float y, float width, float height,int idx) : wallSprite(wallTextures[idx]) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    wallSprite.setPosition({x, y});
    sf::Vector2u textureSize = wallTextures[idx].getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    wallSprite.setScale(scale);
}