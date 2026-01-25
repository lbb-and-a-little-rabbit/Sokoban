#include "Game.h"

Game::Game(unsigned int w,unsigned int h,int cur_level) : player(nullptr),Window_Width(w),Window_Height(h),window(sf::VideoMode({w,h}),"Sokoban"),current_level(cur_level),walkbuffer("assets/walk.ogg"),walksound(walkbuffer) {
    //window.setFramerateLimit(60);
    current_board.current_map=maps_assistant.getMap(current_level); 
    current_board.Init_player_position();
    Set_based_on_board();

    //set music
    if (!music.openFromFile("assets/scattered_and_lost.mp3")) {
        std::cerr << "Music Loading Failed!\n";
        exit(-1);
    }
    music.setLooping(true);
    if (!level_complete.openFromFile("assets/level_win.mp3")) {
        std::cerr << "Music Loading Failed!\n";
        exit(-1);
    }

    //set Text
    if (!uiFont.openFromFile("assets/uifont.ttf")) {
        std::cerr << "Font load failed\n";
        exit(-1);
    }

    // 标题
    sf::Text title(uiFont);
    title.setString("Control");
    title.setCharacterSize(26);
    title.setFillColor(sf::Color::White);
    title.setPosition({20.f, 20.f});
    helpTexts.push_back(title);

    // 内容
    std::vector<std::string> lines = {
        "W / A / S / D    Move",
        "R               Restart",
        "Space           Next Level",
        "H               Hint",
        "P               AI Solver",
        "Tab             Show/Close"
    };

    float y = 60.f;
    for (auto& line : lines) {
        sf::Text t(uiFont);
        t.setString(line);
        t.setCharacterSize(18);
        t.setFillColor(sf::Color(200, 200, 200));
        t.setPosition({20.f, y});
        y += 26.f;
        helpTexts.push_back(t);
    }

}

void Game::run(){
    music.play();
    while(window.isOpen()){
        processEvents();    
        update();
        render();
    }
}

void Game::processEvents(){
    while(const std::optional event=window.pollEvent()){
        if(event->is<sf::Event::Closed>()){
            window.close();
        }

        // 按键处理
        if(event->is<sf::Event::KeyPressed>()){
            Move move;
            bool validMove=true;
            bool gethint=false;
            bool auto_solve=false;

            switch(event->getIf<sf::Event::KeyPressed>()->code){
                case sf::Keyboard::Key::W: move = Move::Up; break;
                case sf::Keyboard::Key::S: move = Move::Down; break;
                case sf::Keyboard::Key::A: move = Move::Left; break;
                case sf::Keyboard::Key::D: move = Move::Right; break;
                case sf::Keyboard::Key::H: {
                    validMove=false;
                    gethint=true;
                    break;
                }
                case sf::Keyboard::Key::P:{
                    isautoSolving=true;
                    validMove=false;
                    auto_solve=true;
                    break;
                }
                case sf::Keyboard::Key::R:{
                    isautoSolving=false;
                    autoSolving=false;
                    validMove=false;
                    islevelCompleted=false;
                    current_board.current_map=maps_assistant.getMap(current_level);
                    current_board.Init_player_position();
                    Set_based_on_board();
                    break;
                }
                case sf::Keyboard::Key::Space:{
                    validMove=false;
                    if(islevelCompleted){
                        islevelCompleted=false;
                        current_level++;
                        if(current_level>=maps_assistant.getLevelCount()){
                            std::cout << "Congratulations!\nYou have completed all levels!\n";
                        }
                        else{
                            current_board.current_map=maps_assistant.getMap(current_level);
                            current_board.Init_player_position();
                            Set_based_on_board();
                        }
                    }
                    break;
                }
                case sf::Keyboard::Key::Tab:{
                    showHelp=!showHelp;
                    validMove=false;
                    break;
                }
                default:
                    validMove=false;
            }

            if(isautoSolving) validMove=false;

            if(validMove){
                auto possibleMoves = current_board.generateMoves();

                if(std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end()){
                    current_board = current_board.applyMove(move);
                    walksound.play();
                    Set_based_on_board();
                }
            }

            if(gethint){
                Solution sol(current_board);
                std::cout << "AI is trying.Please wait...\n";
                Result res=sol.solve(2);
                if(res.timeout){
                    std::cout << "AI can't solve this difficult problem(눈_눈)\nPlease try by yourself!\n";
                }
                else if(res.solvable){
                    std::cout << "This problem is now solvable!\nThe optimal solutions is " << res.best_steps << " step(s).\n";
                }
                else{
                    std::cout << "This problem is not solvable right now!\n";
                }
            }

            if(auto_solve){
                Solution sol(current_board);
                std::cout << "AI is trying.Please wait...\n";
                Result res = sol.solve(2);
                if(res.timeout){
                    std::cout << "AI can't solve this difficult problem(눈_눈)\nPlease try by yourself!\n";
                }
                else if(res.solvable){
                    solutionMoves = res.path;
                    solutionIndex = 0;
                    autoSolving = true;
                    autoSolveClock.restart();
                }
                else{
                    isautoSolving=false;
                    std::cout << "This problem is not solvable right now!\n";
                }
            }
        }

    }
}

void Game::update(){
    if(autoSolving){
        if(solutionIndex >= solutionMoves.size()){
            autoSolving = false;
            isautoSolving=false;
            return;
        }

        if(autoSolveClock.getElapsedTime().asSeconds() >= autoSolveInterval){
            current_board = current_board.applyMove(solutionMoves[solutionIndex]);
            walksound.play();
            solutionIndex++;
            Set_based_on_board();
            autoSolveClock.restart();
        }
    }

    if(current_board.isTerminal()&&!islevelCompleted){
        islevelCompleted=true;
        level_complete.play();
    }
}

void Game::render(){
    window.clear(sf::Color(50, 50, 50));

    for(auto &w:walls) window.draw(w.wallSprite);
    for(auto &t:targets) window.draw(t.targetSprite);
    for(auto &b:boxes) window.draw(b.boxSprite);
    if(player) window.draw(player->playerSprite);
    if (showHelp){
        for(auto& t:helpTexts) window.draw(t);
    }


    window.display();
}

void Game::Clear(){
    delete player;
    player=nullptr;
    boxes.clear();
    walls.clear();
    targets.clear();
}

void Game::Set_based_on_board(){
    Clear();

    std::vector<std::string> cur_map=current_board.current_map;
    int m=cur_map.size();
    int n=cur_map[0].size();
    
    float cellSize=std::min(Window_Width/n,Window_Height/m);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(cur_map[i][j]=='#'){
                walls.emplace_back(j*cellSize,i*cellSize,cellSize,cellSize);
            }
            else if(cur_map[i][j]=='x'){
                targets.emplace_back(j*cellSize,i*cellSize,cellSize,cellSize);
            }
            else if(cur_map[i][j]=='B'||cur_map[i][j]=='b'){
                boxes.emplace_back(j*cellSize,i*cellSize,cellSize,cellSize,cur_map[i][j]=='B');
            }
            else if(cur_map[i][j]=='P'||cur_map[i][j]=='p'){
                player=new Player(j*cellSize,i*cellSize,cellSize,cellSize);
            }
        }
    }
}