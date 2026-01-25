#include "Game.h"

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"SoKoban");
    Game game(window,800,600,0);
    game.run();
}