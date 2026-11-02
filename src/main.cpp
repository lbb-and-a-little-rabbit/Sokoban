#include "DisplayScaling.h"
#include "GameWindowInput.h"
#include "Game.h"
#include "Menu.h"
#include "CGs.h"
#include "LevelSelect.h"
#include "EncryptedPak.h"
#include <ctime>
#include <filesystem>

#include <physfs.h>

int main(int argc, char** argv){
    DisplayScaling::enableDpiAwareness();
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

    // Start fullscreen at the display's current resolution.
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Sokoban", sf::State::Fullscreen);
    DisplayScaling::updateView(window);
    GameWindowInput gameInput(window);

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
            if(DisplayScaling::handleEvent(window, *event)) continue;
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
        Ground::LoadTextures();
        Box::LoadTextures();
        Player::LoadTextures();
        Target::LoadTextures();
        Wall::LoadTextures();
        LevelSelect::LoadTextures();
        Menu::LoadTextures();
        Game::LoadTextures();

        assetsLoaded = true;
    });
    cgs.run(assetsLoaded);
    loader.join();
    if (!window.isOpen()) return 0;

    // One menu owns one music stream for the entire navigation session.
    Menu menu(window);
    while(window.isOpen()) {
        const MenuResult result = menu.run();
        if(result == MenuResult::Exit) {
            window.close();
            break;
        }
        int selectedLevel = 0;
        if(result == MenuResult::SelectLevel) {
            LevelSelect select(window, menu.maxLevel);
            const auto [selection, level] = select.run();
            if(selection == LevelResult::Close) {
                window.close();
                break;
            }
            if(selection != LevelResult::Selected) continue;
            selectedLevel = level;
        } else if(result != MenuResult::StartGame) {
            continue;
        }
        // Pause only when entering gameplay; returning resumes the same track.
        menu.currentMusic.pause();
        Game game(window,1920,1200,selectedLevel);
        game.run();
    }

    std::filesystem::remove_all("temp");
    PHYSFS_deinit();
}
