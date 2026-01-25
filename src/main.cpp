#include "Game.h"
#include "Menu.h"
#include "LevelSelect.h"

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"SoKoban");
menu_label:
    Menu menu(window);
    MenuResult result=menu.run();

    if(result==MenuResult::Exit){
        window.close();
        return 0;
    }

    if(result==MenuResult::StartGame){
        Game game(window,800,600,0);
        game.run();
        goto menu_label;
    }

    if(result==MenuResult::SelectLevel){
        LevelSelect ls(window,menu.maxLevel);
        std::pair<LevelResult,int> res=ls.run();

        if(res.first==LevelResult::Close){
            window.close();
            return 0;
        }

        if(res.first==LevelResult::Exit) goto menu_label;

        if(res.first==LevelResult::Selected){
            menu.currentMusic.stop();
            Game game(window,800,600,res.second);
            game.run();
            goto menu_label;
        }
    }
}