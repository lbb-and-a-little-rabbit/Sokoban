#include "Game.h"
#include "Menu.h"

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"SoKoban");
    Menu menu(window);
    MenuResult result=menu.run();

    if(result==MenuResult::Exit){
        window.close();
        return 0;
    }

    if(result==MenuResult::StartGame){
        Game game(window,800,600,0);
        game.run();
    }
}