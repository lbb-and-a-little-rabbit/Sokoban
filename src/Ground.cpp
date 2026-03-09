#include "Ground.h"

std::vector<sf::Texture> Ground::groundTextures;

void Ground::LoadTextures(){
    if (groundTextures.empty()) {
        auto data = LoadFile("assets/ground.png");
        sf::Texture tex;
        if (!tex.loadFromMemory(data.data(), data.size())) {
            std::cerr << "Failed to load ground.png\n";
            exit(-1);
        }
        groundTextures.push_back(std::move(tex));
    }
}

Ground::Ground(float x, float y, float width, float height) : groundSprite(groundTextures[0]) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    groundSprite.setPosition({x, y});
    sf::Vector2u textureSize = groundSprite.getTexture().getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    groundSprite.setScale(scale);
}