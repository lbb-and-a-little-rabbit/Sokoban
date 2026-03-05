#include "Game.h"
#include "Menu.h"
#include "CGs.h"
#include "LevelSelect.h"
#include "EncryptedPak.h"
#include <ctime>
#include <filesystem>

#include <physfs.h>

int main(int argc, char** argv){
    std::cout << "loading...";

    PHYSFS_init(nullptr);

    // 允许读当前目录
    PHYSFS_mount(".", nullptr, 0);

    // 设置写入目录
    PHYSFS_setWriteDir(".");

    if (!MountEncryptedPak("data.sxk")) {
        std::cout << "Failed to mount encrypted pak\n";
        return -1;
    }

    srand((unsigned)time(nullptr));

    sf::RenderWindow window(sf::VideoMode({800,600}),"SoKoban");
    sf::View view(sf::FloatRect({0.f,0.f},{(float)window.getSize().x,(float)window.getSize().y}));
    window.setView(view);

    CGs::LoadTextures();

    // 加载图标
    sf::Image icon;
    auto icondata=LoadFile("assets/target_box.png");
    if (!icon.loadFromMemory(icondata.data(),icondata.size())) {
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

    CGs cgs(window);
    std::atomic<bool> assetsLoaded = false;
    std::thread loader([&](){

        //INIT_ASSETS
        Box::LoadTextures();
        Player::LoadTextures();
        Target::LoadTextures();
        Wall::LoadTextures();
        LevelSelect::LoadTextures();
        Menu::LoadTextures();
        Game::LoadTextures();

        //DecryptToTempFile("assets/temple.wav");
        //INIT_ASSETS

        assetsLoaded = true;
    });
    cgs.run(assetsLoaded);
    loader.join();

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

    std::filesystem::remove_all("temp");
    PHYSFS_deinit();
}