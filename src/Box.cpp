#include "Box.h"

std::vector<sf::Texture> Box::n_boxTextures;
std::vector<sf::Texture> Box::t_boxTextures;

void Box::LoadTextures() {
    if (n_boxTextures.empty()) {
        auto data = LoadFile("assets/normal_box.png");
        sf::Texture tex;
        if (!tex.loadFromMemory(data.data(), data.size())) {
            std::cerr << "Failed to load normal_box.png\n";
            exit(-1);
        }
        n_boxTextures.push_back(std::move(tex));
    }

    if (t_boxTextures.empty()) {
        auto data = LoadFile("assets/target_box.png");
        sf::Texture tex;
        if (!tex.loadFromMemory(data.data(), data.size())) {
            std::cerr << "Failed to load target_box.png\n";
            exit(-1);
        }
        t_boxTextures.push_back(std::move(tex));
    }
}

Box::Box(float x, float y, float width, float height,bool status) : boxSprite(status?t_boxTextures[0] : n_boxTextures[0]) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    boxSprite.setPosition({x, y});
    sf::Vector2u textureSize = boxSprite.getTexture().getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    boxSprite.setScale(scale);
}