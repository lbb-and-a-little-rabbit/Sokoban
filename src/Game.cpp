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
            bool validMove = true;

            switch(event->getIf<sf::Event::KeyPressed>()->code){
                case sf::Keyboard::Key::W: move = Move::Up; break;
                case sf::Keyboard::Key::S: move = Move::Down; break;
                case sf::Keyboard::Key::A: move = Move::Left; break;
                case sf::Keyboard::Key::D: move = Move::Right; break;
                default:
                    validMove = false;
            }

            if(validMove){
                auto possibleMoves = current_board.generateMoves();

                if(std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end()){
                    current_board = current_board.applyMove(move);
                    Set_based_on_board();
                }
            }
        }

    }
}

void Game::update(){
    
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