#include "Game.h"

Game::Game(unsigned int w,unsigned int h,int cur_level) : Window_Width(w),Window_Height(h),window(sf::VideoMode({w,h}),"Sokoban"),current_level(cur_level) {
    window.setFramerateLimit(60);
    current_board.current_map=maps_assistant.getMap(current_level); 
    current_board.Init_player_position();
    Set_based_on_board();
}

void Game::run(){
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
                }
                case sf::Keyboard::Key::P:{
                    validMove=false;
                    auto_solve=true;
                }
                default:
                    validMove=false;
            }

            if(validMove){
                auto possibleMoves = current_board.generateMoves();

                if(std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end()){
                    current_board = current_board.applyMove(move);
                    Set_based_on_board();
                }
            }

            if(gethint){
                Solution sol(current_board);
                Result res=sol.solve(2);
                if(res.solvable){
                    std::cout << "This problem is now solvable!\nThe optimal solutions is " << res.best_steps << " step(s).\n";
                }
                else{
                    std::cout << "This problem is not solvable right now!\n";
                }
                break;
            }

            if(auto_solve){
                Solution sol(current_board);
                Result res = sol.solve(2);

                if(res.solvable){
                    solutionMoves = res.path;
                    solutionIndex = 0;
                    autoSolving = true;
                    autoSolveClock.restart();
                }
                else{
                    std::cout << "This problem is not solvable right now!\n";
                }
                break;
            }
        }

    }
}

void Game::update(){
    if(autoSolving){
        if(solutionIndex >= solutionMoves.size()){
            autoSolving = false;
            return;
        }

        if(autoSolveClock.getElapsedTime().asSeconds() >= autoSolveInterval){
            current_board = current_board.applyMove(solutionMoves[solutionIndex]);
            solutionIndex++;
            Set_based_on_board();
            autoSolveClock.restart();
        }
    }
}

void Game::render(){
    window.clear(sf::Color(50, 50, 50));

    for(auto &w:walls) window.draw(w.wallSprite);
    for(auto &t:targets) window.draw(t.targetSprite);
    for(auto &b:boxes) window.draw(b.boxSprite);
    if(player) window.draw(player->playerSprite);

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