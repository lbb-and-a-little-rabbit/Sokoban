#include "DisplayScaling.h"
#include "Game.h"
#include <algorithm>
#include <chrono>

sf::SoundBuffer Game::walkbuffer;
std::vector<char> Game::fontData;
std::vector<char> Game::bgmData;
std::vector<char> Game::level_comData;

void Game::LoadTextures(){
    auto bufferdata=LoadFile("assets/walk.ogg");
    if(!walkbuffer.loadFromMemory(bufferdata.data(),bufferdata.size())){
        std::cerr << "Failed to load!";
        exit(-1);
    }

    fontData=LoadFile("assets/uifont.ttf");

    bgmData=LoadFile("assets/scattered_and_lost.mp3");

    level_comData=LoadFile("assets/level_win.mp3");
}

Game::Game(sf::RenderWindow &window,unsigned int w,unsigned int h,int cur_level) : Window_Width(w),Window_Height(h),window(window),current_level(cur_level),player(nullptr),walksound(walkbuffer) {
    //window.setFramerateLimit(60);
    current_board.current_map=maps_assistant.getMap(current_level); 
    current_board.Init_player_position();
    Set_based_on_board(Move::Down);

    //set music
    if (!music.openFromMemory(bgmData.data(),bgmData.size())) {
        std::cerr << "Music Loading Failed!\n";
        exit(-1);
    }
    music.setLooping(true);
    if (!level_complete.openFromMemory(level_comData.data(),level_comData.size())) {
        std::cerr << "Music Loading Failed!\n";
        exit(-1);
    }

    //set Text
    if (!uiFont.openFromMemory(fontData.data(),fontData.size())) {
        std::cerr << "Font load failed\n";
        exit(-1);
    }

    // 标题
    sf::Text title(uiFont);
    title.setString(L"按键说明");
    title.setCharacterSize(26);
    title.setFillColor(sf::Color::White);
    title.setPosition({20.f, 20.f});
    helpTexts.push_back(title);

    // 内容
    std::vector<std::wstring> lines = {
        L"W / A / S / D\t移动",
        L"R            \t重新开始",
        L"Space        \t通关后进入下一关",
        L"H            \t提示",
        L"P            \tAI解决",
        L"Tab          \t打开/关闭该界面",
        L"Esc          \t返回（游戏中返回菜单）"
    };

    float y = 60.f;
    for (auto& line : lines) {
        sf::Text t(uiFont);
        t.setString(line);
        t.setCharacterSize(36);
        t.setFillColor(sf::Color(200, 200, 200));
        t.setPosition({20.f, y});
        y += 52.f;
        helpTexts.push_back(t);
    }

}


Game::~Game(){
    if(aiResult.valid()) aiResult.wait();
    Clear();
}

void Game::run(){
    music.play();
    window.setKeyRepeatEnabled(false);
    while(window.isOpen() && !backtomenu){
        processEvents();
        if(!window.isOpen() || backtomenu) break;
        update();
        render();
        sf::sleep(sf::milliseconds(10));
    }
    window.setKeyRepeatEnabled(true);
    music.stop();
}

void Game::startAI(bool playback){
    aiRequestedPlayback = playback;
    dialogMessage.clear();
    aiThinking = true;
    // The worker owns a board snapshot and never touches SFML or game state.
    try {
        aiResult = std::async(std::launch::async, [board = current_board](){
            return Solution(board).solve(2);
        });
    } catch (...) {
        aiThinking = false;
        dialogMessage = L"AI思考失败，请自己试试";
    }
}

void Game::processEvents(){
    while(const auto event = window.pollEvent()){
            if(DisplayScaling::handleEvent(window, *event)) continue;
        if(event->is<sf::Event::Closed>()){
            window.close();
            return;
        }
        const auto* key = event->getIf<sf::Event::KeyPressed>();
        if(!key) continue;
        // Consume (never queue) every gameplay command while AI owns the board.
        if(aiThinking || autoSolving) continue;
        if(!dialogMessage.empty()){
            if(key->scancode == sf::Keyboard::Scancode::Enter || key->scancode == sf::Keyboard::Scancode::Escape)
                dialogMessage.clear();
            continue;
        }
        if(key->scancode == sf::Keyboard::Scancode::Tab){
            showHelp = !showHelp;
            continue;
        }
        if(showHelp){
            if(key->scancode == sf::Keyboard::Scancode::Escape) showHelp = false;
            continue;
        }
        Move move = Move::None;
        switch(key->scancode){
            case sf::Keyboard::Scancode::W: move = Move::Up; break;
            case sf::Keyboard::Scancode::S: move = Move::Down; break;
            case sf::Keyboard::Scancode::A: move = Move::Left; break;
            case sf::Keyboard::Scancode::D: move = Move::Right; break;
            case sf::Keyboard::Scancode::H: startAI(false); break;
            case sf::Keyboard::Scancode::P: startAI(true); break;
            case sf::Keyboard::Scancode::Escape: backtomenu = true; return;
            case sf::Keyboard::Scancode::R:
                islevelCompleted = false;
                current_board.current_map = maps_assistant.getMap(current_level);
                current_board.Init_player_position();
                Set_based_on_board(Move::Down);
                break;
            case sf::Keyboard::Scancode::Space:
                if(islevelCompleted){
                    if(current_level + 1 >= maps_assistant.getLevelCount()){
                        dialogMessage = L"恭喜通关，你做到了！\n感谢游玩";
                    } else {
                        ++current_level;
                        islevelCompleted = false;
                        current_board.current_map = maps_assistant.getMap(current_level);
                        current_board.Init_player_position();
                        Set_based_on_board(Move::Down);
                    }
                }
                break;
            default: break;
        }
        if(move != Move::None && !islevelCompleted && current_board.canMove(move)){
            current_board = current_board.applyMove(move);
            walksound.play();
            Set_based_on_board(move);
        }
    }
}

void Game::update(){
    if(aiThinking){
        if(aiResult.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;
        aiThinking = false;
        try {
            const Result res = aiResult.get();
            if(res.timeout){
                dialogMessage = L"AI思考失败，请自己试试";
            } else if(!res.solvable){
                dialogMessage = L"当前局面已无解";
            } else if(aiRequestedPlayback){
                solutionMoves = res.path;
                solutionIndex = 0;
                autoSolving = !solutionMoves.empty();
                autoSolveClock.restart();
            } else {
                dialogMessage = L"AI已找到解法，共 " + std::to_wstring(res.best_steps) + L" 步";
            }
        } catch (...) {
            dialogMessage = L"AI思考失败，请自己试试";
        }
    }
    if(autoSolving && autoSolveClock.getElapsedTime().asSeconds() >= autoSolveInterval){
        const Move move = solutionMoves[solutionIndex++];
        current_board = current_board.applyMove(move);
        walksound.play();
        Set_based_on_board(move);
        autoSolveClock.restart();
        if(solutionIndex >= solutionMoves.size()) autoSolving = false;
    }
    if(current_board.isTerminal() && !islevelCompleted){
        islevelCompleted = true;
        level_complete.play();
        if(current_level == maps_assistant.getLevelCount() - 1)
            dialogMessage = L"恭喜通关，你做到了！\n感谢游玩";
        SaveData data = SaveSystem::load();
        data.maxUnlockedLevel = std::max(data.maxUnlockedLevel,
            std::min(current_level + 1, maps_assistant.getLevelCount() - 1));
        SaveSystem::save(data);
    }
}

void Game::drawDialog(const std::wstring& message, bool dismissible){
    const auto size = window.getView().getSize();
    const auto center = window.getView().getCenter();
    sf::RectangleShape shade(size);
    shade.setPosition(center - size / 2.f);
    shade.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(shade);
    sf::Text text(uiFont, message, 36);
    const auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);
    text.setPosition(center - sf::Vector2f(0.f, dismissible ? 22.f : 0.f));
    sf::RectangleShape panel({std::max(500.f, bounds.size.x + 90.f), std::max(dismissible ? 180.f : 130.f, bounds.size.y + (dismissible ? 110.f : 60.f))});
    panel.setOrigin(panel.getSize() / 2.f);
    panel.setPosition(center);
    panel.setFillColor(sf::Color(35, 42, 55));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(160, 180, 210));
    window.draw(panel);
    window.draw(text);
    if(dismissible){
        sf::Text footer(uiFont, L"Enter / Esc 关闭", 24);
        const auto b = footer.getLocalBounds();
        footer.setOrigin(b.position + b.size / 2.f);
        footer.setPosition(center + sf::Vector2f(0.f, panel.getSize().y / 2.f - 35.f));
        window.draw(footer);
    }
}

void Game::render(){
    window.clear(sf::Color(128, 128, 128));
    const auto size = window.getView().getSize();
    const auto center = window.getView().getCenter();
    if(showHelp){
        window.clear(sf::Color(25, 32, 45));
        for(size_t i = 0; i < helpTexts.size(); ++i){
            auto& text = helpTexts[i];
            text.setPosition(center + sf::Vector2f(-300.f, -240.f + i * 60.f));
            window.draw(text);
        }
    } else {
        for(auto &g:grounds) window.draw(g.groundSprite);
        for(auto &w:walls) window.draw(w.wallSprite);
        for(auto &t:targets) window.draw(t.targetSprite);
        for(auto &b:boxes) window.draw(b.boxSprite);
        if(player) window.draw(player->playerSprite);
        sf::Text status(uiFont, autoSolving ? L"AI自走中" :
            (islevelCompleted ? (current_level + 1 == maps_assistant.getLevelCount() ?
                L"全部通关 · Esc 返回菜单 · R 重玩" : L"关卡完成 · Space 下一关 · Tab 按键说明") : L"Tab 按键说明"), 24);
        status.setPosition(center - size / 2.f + sf::Vector2f(20.f, 12.f));
        window.draw(status);
    }
    if(aiThinking) drawDialog(L"AI思考中", false);
    else if(!dialogMessage.empty()) drawDialog(dialogMessage, true);
    window.display();
}

void Game::Clear(){
    delete player;
    player = nullptr;
    boxes.clear();
    walls.clear();
    targets.clear();
    grounds.clear();
}

void Game::Set_based_on_board(Move move){
    Clear();
    const auto& cur_map = current_board.current_map;
    const int m = cur_map.size();
    const int n = cur_map[0].size();
    const auto size = window.getView().getSize();
    const auto center = window.getView().getCenter();
    const float cellSize = std::min((size.x - 80.f) / n, (size.y - 100.f) / m);
    const float offsetX = center.x - n * cellSize / 2.f;
    const float offsetY = center.y - m * cellSize / 2.f;
    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            const float x = offsetX + j * cellSize;
            const float y = offsetY + i * cellSize;
            grounds.emplace_back(x,y,cellSize,cellSize);
            if(cur_map[i][j]=='#')
                walls.emplace_back(x,y,cellSize,cellSize,(i==0||cur_map[i-1][j]!='#')?0:1);
            else if(cur_map[i][j]=='x')
                targets.emplace_back(x,y,cellSize,cellSize);
            else if(cur_map[i][j]=='B'||cur_map[i][j]=='b')
                boxes.emplace_back(x,y,cellSize,cellSize,cur_map[i][j]=='B');
            else if(cur_map[i][j]=='P'||cur_map[i][j]=='p')
                player = new Player(x,y,cellSize,cellSize,(PlayerForward)((int)move));
        }
    }
}
