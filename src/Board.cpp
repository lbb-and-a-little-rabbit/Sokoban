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
    for(const auto& row:current_map) {s+=row;s.push_back('\n');}
    return s;
}

bool Board::isDeadCorner(int x, int y) {
    char c = current_map[y][x];

    // 如果是目标，不算死角
    if(c == 'B' || c == 'x') return false;

    int m = current_map.size();
    int n = current_map[0].size();

    auto wall = [&](int yy, int xx){
        if(yy<0 || yy>=m || xx<0 || xx>=n) return true;
        char ch = current_map[yy][xx];
        return ch=='#';
    };

    // 1. 四角死角
    if((wall(y-1,x) && wall(y,x-1)) ||
       (wall(y-1,x) && wall(y,x+1)) ||
       (wall(y+1,x) && wall(y,x-1)) ||
       (wall(y+1,x) && wall(y,x+1))) return true;

    // 2. 墙边死角（水平或垂直夹墙）
    // 垂直夹墙
    if((wall(y-1,x) && wall(y+1,x)) && 
       (x>0 && current_map[y][x-1] != 'x') && 
       (x<n-1 && current_map[y][x+1] != 'x')) return true;

    // 水平夹墙
    if((wall(y,x-1) && wall(y,x+1)) && 
       (y>0 && current_map[y-1][x] != 'x') && 
       (y<m-1 && current_map[y+1][x] != 'x')) return true;

    // 3. 可选：箱子排列死局（相邻箱子无法推动）  
    // 例如两个箱子沿墙角排成 L 型且不是目标，可以加一个规则  

    return false;
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

bool Board::canMove(Move m){
    for(Move move:generateMoves()){
        if(move==m) return true;
    }
    return false;
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

int Board::sumBoxToTargetDist(){
    auto manhattandist=[](const std::pair<int,int> &a, const std::pair<int,int> &b) -> int {
        return abs(a.first - b.first) + abs(a.second - b.second);
    };
    std::vector<std::pair<int,int>> boxes,targets;
    int m=current_map.size();
    int n=current_map[0].size();
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(current_map[i][j]=='b') boxes.push_back({i,j});
            if(current_map[i][j]=='x'||current_map[i][j]=='P') targets.push_back({i,j});
        }
    }
    int dist=0;
    for(auto &b:boxes){
        int best=INT_MAX;
        for(auto &t:targets){
            best=std::min(best,manhattandist(b,t));
        }
    }
    return dist;
}

bool Board::boxOnTargetIncreased(Board &b){
    int cnt1=0,cnt2=0;
    std::string s1=MaptoString(),s2=b.MaptoString();
    for(int i=0;i<s1.size();i++){
        if(s1[i]=='B') cnt1++;
        if(s2[i]=='B') cnt2++;
    }
    return cnt1>cnt2;
}

//Dead判断

inline bool isWall(char c){
    return c=='#';
}

inline bool isBox(char c){
    return c=='b' || c=='B';
}

inline bool isTarget(char c){
    return c=='x' || c=='B' || c=='P';
}

bool Board::check2x2Dead(int y, int x){
    static int dx[4]={0,1,0,1};
    static int dy[4]={0,0,1,1};

    int m=current_map.size();
    int n=current_map[0].size();

    for(int k=0;k<4;k++){
        int sy=y-dy[k];
        int sx=x-dx[k];
        if(sy<0||sx<0||sy+1>=m||sx+1>=n) continue;

        bool allBlock=true;
        bool hasNonTargetBox=false;

        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++){
                char c=current_map[sy+i][sx+j];
                if(!(isWall(c)||isBox(c))){
                    allBlock=false;
                }
                if(c=='b') hasNonTargetBox=true;
            }
        }

        if(allBlock && hasNonTargetBox) return true;
    }
    return false;
}

bool Board::frozenOnWall(int y,int x){
    int m=current_map.size();
    int n=current_map[0].size();

    // 贴左 / 右墙
    if(isWall(current_map[y][x-1]) || isWall(current_map[y][x+1])){
        bool hasTarget=false;
        for(int j=0;j<n;j++){
            if(current_map[y][j]=='x'||current_map[y][j]=='B') {
                hasTarget=true; break;
            }
        }
        if(!hasTarget) return true;
    }

    // 贴上 / 下墙
    if(isWall(current_map[y-1][x]) || isWall(current_map[y+1][x])){
        bool hasTarget=false;
        for(int i=0;i<m;i++){
            if(current_map[i][x]=='x'||current_map[i][x]=='B'){
                hasTarget=true; break;
            }
        }
        if(!hasTarget) return true;
    }
    return false;
}


bool Board::isDead(){
    int m=current_map.size();
    int n=current_map[0].size();

    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(current_map[i][j]=='b'){   // 非目标箱子
                // 角落死锁
                if(isDeadCorner(j,i)) return true;

                // 2x2 block
                if(check2x2Dead(i,j)) return true;

                // 贴墙冻结
                if(frozenOnWall(i,j)) return true;
            }
        }
    }
    return false;
}
