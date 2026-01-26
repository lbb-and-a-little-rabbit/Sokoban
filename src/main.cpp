#include "Game.h"
#include "Menu.h"
#include "LevelSelect.h"
#include <ctime>

int main(int argc, char** argv){
    srand((unsigned)time(nullptr));

    sf::RenderWindow window(sf::VideoMode({800,600}),"SoKoban");

    // 加载图标
    sf::Image icon;
    if (!icon.loadFromFile("assets/target_box.png")) {
        // 加载失败可用默认
        std::cout << "Failed to load icon.png!" << std::endl;
    } else {
        window.setIcon(icon);
    }

    if(argc > 1 && std::string(argv[1]) == "--train"){
        TrainingStatus status;
        Trainer t;
        t.totalEpisodes = 5000;
        t.trainAsync(&status);

        // 窗口显示训练信息
        sf::Font font;
        bool isfontload=font.openFromFile("assets/uifont.ttf"); // 确保有字体文件
        sf::Text text(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition({50,50});

        while(window.isOpen()){
            while(const std::optional event=window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    window.close();
                }
            }

            window.clear();

            // 更新训练信息
            std::string info = "Episode: " + std::to_string(status.currentEpisode.load()) +
                               "/" + std::to_string(t.totalEpisodes) +
                               "\nSolved: " + std::to_string(status.solved.load()) +
                               "\nAvgReward: " + std::to_string(status.avgReward.load());
            text.setString(info);

            window.draw(text);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        return 0;
    }

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