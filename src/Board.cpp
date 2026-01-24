#include "Board.h"

Board::Board(){
    player_position={-1,-1};
}

void Board::Init_player_position(){
    if(!current_map.size()){
        std::cerr << "Failed to initialize player position!\n";
        exit(-1);
    }

    bool find=false;
    for(int i=0;i<current_map.size();i++){
        if(find) break;
        for(int j=0;j<current_map[0].size();j++){
            if(current_map[i][j]=='p'||current_map[i][j]=='P'){
                player_position={i,j};
                find=true;
                break;
            }
        }
    }
}

std::string Board::MaptoString(){
    std::string s;
    for(const auto& row:current_map) s+=row;
    return s;
}

std::vector<Move> Board::generateMoves(){
    int m=current_map.size();
    int n=current_map[0].size();
    int x=player_position.first;
    int y=player_position.second;
    std::vector<Move> moves;
    for(int i=0;i<4;i++){
        int nx=x+dir[i][0];
        int ny=y+dir[i][1];
        if(nx>=m||nx<0||ny>=n||ny<0) continue;
        if(current_map[nx][ny]==' '||current_map[nx][ny]=='x'){
            moves.push_back((Move)i);
            continue;
        }
        if(current_map[nx][ny]=='#') continue;
        if(current_map[nx][ny]=='b'||current_map[nx][ny]=='B'){
            int nnx=nx+dir[i][0];
            int nny=ny+dir[i][1];
            if(nnx>=m||nnx<0||nny>=n||nny<0) continue;
            if(current_map[nnx][nny]==' '||current_map[nnx][nny]=='x') moves.push_back((Move)i);
        }
    }
    return moves;
}

Board Board::applyMove(Move move){
    Board res;
    res.current_map=current_map;
    res.player_position.first=player_position.first+dir[(int)move][0];
    res.player_position.second=player_position.second+dir[(int)move][1];
    int px=res.player_position.first;
    int py=res.player_position.second;

    if(current_map[player_position.first][player_position.second]=='p') res.current_map[player_position.first][player_position.second]=' ';
    else res.current_map[player_position.first][player_position.second]='x';

    if(current_map[px][py]==' '||current_map[px][py]=='b') res.current_map[px][py]='p';
    else res.current_map[px][py]='P';

    if(current_map[px][py]=='b'||current_map[px][py]=='B'){
        int nnx=px+dir[(int)move][0];
        int nny=py+dir[(int)move][1];
        if(current_map[nnx][nny]==' ') res.current_map[nnx][nny]='b';
        else res.current_map[nnx][nny]='B';
    }

    return res;
}

bool Board::isTerminal(){
    int m=current_map.size();
    int n=current_map[0].size();
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(current_map[i][j]=='x') return false;
            if(current_map[i][j]=='P') return false;
        }
    }
    return true;
}