#include "Box.h"

sf::Texture nbox1("assets/normal_box.png");

sf::Texture tbox1("assets/target_box.png");

std::vector<sf::Texture> Box::n_boxTextures;
std::vector<sf::Texture> Box::t_boxTextures;

Box::Box(float x, float y, float width, float height,bool status) : boxSprite(status?tbox1:nbox1) {
    shape.setSize({width, height});
    shape.setPosition({x, y});

    if(n_boxTextures.size()<1){
        n_boxTextures.push_back(nbox1);
    }
    if(t_boxTextures.size()<1){
        t_boxTextures.push_back(tbox1);
    }

    boxSprite.setPosition({x, y});
    sf::Vector2u textureSize = boxSprite.getTexture().getSize();
    sf::Vector2f windowSize = shape.getSize();
    sf::Vector2f scale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );
    boxSprite.setScale(scale);
}