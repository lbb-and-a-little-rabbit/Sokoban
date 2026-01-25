#include "Solution.h"

Solution::Solution(int idx,Map &m){
    current_board.current_map=m.getMap(idx);
    current_board.Init_player_position();
}

Solution::Solution(std::vector<std::string> cur_map){
    current_board.current_map=cur_map;
    current_board.Init_player_position();
}

Solution::Solution(Board board){
    current_board=board;
}

Result Solution::solve(int choice){
    if(choice==1){
        return UCS();
    }
    if(choice==2){
        return Astar();
    }
    std::cerr << "Failed to find solution\n";
    exit(-1);
}

Result Solution::UCS(){
    Result res;

    auto start = std::chrono::steady_clock::now();

    std::unordered_map<std::string,std::string> parent;
    std::unordered_map<std::string,Move> parentMove;
    std::unordered_map<std::string,int> dist;
    auto cmp=[](const std::pair<int, Board>& a,const std::pair<int, Board>& b){
        return a.first > b.first;
    };
    std::priority_queue<std::pair<int,Board>,std::vector<std::pair<int,Board>>,decltype(cmp)> pq(cmp);
    dist[current_board.MaptoString()]=0;
    pq.push({0,current_board});
    while(!pq.empty()){

        // ===== 超时检测 =====
        auto now = std::chrono::steady_clock::now();
        double elapsed =
            std::chrono::duration<double>(now - start).count();
        if(elapsed >= TIME_LIMIT){
            res.timeout = true;
            res.solvable = false;
            return res;
        }
        // ====================

        auto [w,board]=pq.top();
        pq.pop();

        std::string code=board.MaptoString();
        if(dist.count(code)&&dist[code]<w) continue;

        if(board.isTerminal()){
            res.solvable=true;
            res.best_steps=w;

            std::vector<Move> path;
            std::string cur=code;
            while(parent.count(cur)){
                path.push_back(parentMove[cur]);
                cur=parent[cur];
            }
            std::reverse(path.begin(),path.end());
            res.path=path;

            return res;
        }

        for(Move move:board.generateMoves()){
            Board newBoard=board.applyMove(move);
            std::string newCode=newBoard.MaptoString();
            int wt=w+1;
            if(!dist.count(newCode)||wt<dist[newCode]){
                dist[newCode]=wt;
                parent[newCode]=code; 
                parentMove[newCode]=move;
                pq.push({wt,newBoard});
            }
        }
    }
    res.solvable=false;
    return res;
}

//Assistant Function
int manhattan(const std::pair<int,int> &a, const std::pair<int,int> &b){
    return abs(a.first - b.first) + abs(a.second - b.second);
}

// Hungarian Algorithm for minimum cost perfect matching
int hungarian(const std::vector<std::vector<int>> &cost) {
    int n = cost.size(); // 假设 n x n
    std::vector<int> u(n+1), v(n+1), p(n+1), way(n+1);
    const int INF = 1e9;

    for(int i=1;i<=n;i++){
        p[0]=i;
        std::vector<int> minv(n+1, INF);
        std::vector<bool> used(n+1,false);
        int j0=0;
        do{
            used[j0]=true;
            int i0=p[j0], delta=INF, j1=0;
            for(int j=1;j<=n;j++){
                if(!used[j]){
                    int cur = cost[i0-1][j-1]-u[i0]-v[j];
                    if(cur<minv[j]){
                        minv[j]=cur;
                        way[j]=j0;
                    }
                    if(minv[j]<delta){
                        delta=minv[j];
                        j1=j;
                    }
                }
            }
            for(int j=0;j<=n;j++){
                if(used[j]){
                    u[p[j]]+=delta;
                    v[j]-=delta;
                }else{
                    minv[j]-=delta;
                }
            }
            j0=j1;
        }while(p[j0]!=0);

        do{
            int j1=way[j0];
            p[j0]=p[j1];
            j0=j1;
        }while(j0!=0);
    }

    return -v[0]; // 最小总成本
}

// Sokoban 最小匹配启发函数
int heuristic_min_matching(const std::vector<std::pair<int,int>> &boxes,
                           const std::vector<std::pair<int,int>> &targets) {
    int n = boxes.size();
    int m = targets.size();
    int size = std::max(n,m); // 矩阵大小，补虚拟行列

    std::vector<std::vector<int>> cost(size, std::vector<int>(size, 0));

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cost[i][j] = manhattan(boxes[i], targets[j]);
        }
    }
    // 多出的行列默认填 0，表示虚拟箱子/目标，不影响总成本

    return hungarian(cost);
}
//

Result Solution::Astar(){
    Result res;

    auto start = std::chrono::steady_clock::now();

    std::unordered_map<std::string,std::string> parent;
    std::unordered_map<std::string,Move> parentMove;
    std::unordered_map<std::string,int> dist;

    auto heuristic=[](Board &b) -> int{
        int h=0;

        int m=b.current_map.size();
        int n=b.current_map[0].size();
        std::vector<std::pair<int,int>> boxPoses;
        std::vector<std::pair<int,int>> targetPoses;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(b.current_map[i][j]=='b'){
                    boxPoses.push_back({i,j});
                }
                if(b.current_map[i][j]=='P'||b.current_map[i][j]=='x'){
                    targetPoses.push_back({i,j});
                }
            }
        }

        h = heuristic_min_matching(boxPoses, targetPoses);

        // 玩家距离加成
        auto [px,py] = b.player_position;
        int player_dist = INT_MAX;
        for(auto [by,bx]:boxPoses){
            player_dist = std::min(player_dist, abs(px-bx)+abs(py-by));
        }
        h += player_dist;

        // 死局惩罚
        for(auto [by,bx]:boxPoses){
            if(b.isDeadCorner(bx,by)) h += 1000;
        }

        return h;
    };

    auto cmp=[](const std::pair<int, Board>& a,const std::pair<int, Board>& b){
        return a.first > b.first;
    };
    std::priority_queue<std::pair<int,Board>,std::vector<std::pair<int,Board>>,decltype(cmp)> pq(cmp);
    dist[current_board.MaptoString()]=0;
    pq.push({heuristic(current_board),current_board});
    while(!pq.empty()){

        // ===== 超时检测 =====
        auto now = std::chrono::steady_clock::now();
        double elapsed =
            std::chrono::duration<double>(now - start).count();
        
        //std::cout << "Time used " << elapsed << "s\n";
        
        if(elapsed >= TIME_LIMIT){
            res.timeout = true;
            res.solvable = false;
            return res;
        }
        // ====================

        auto [f,board]=pq.top();
        pq.pop();

        std::string code=board.MaptoString();
        int g=dist[code];

        if(board.isTerminal()){
            res.solvable=true;
            res.best_steps=g;

            std::vector<Move> path;
            std::string cur=code;
            while(parent.count(cur)){
                path.push_back(parentMove[cur]);
                cur=parent[cur];
            }
            std::reverse(path.begin(),path.end());
            res.path=path;

            return res;
        }

        for(Move move:board.generateMoves()){
            Board newBoard=board.applyMove(move);
            std::string newCode=newBoard.MaptoString();
            int wt=g+1;
            if(!dist.count(newCode)||wt<dist[newCode]){
                dist[newCode]=wt;
                parent[newCode]=code; 
                parentMove[newCode]=move;
                pq.push({wt+heuristic(newBoard),newBoard});
            }
        }
    }
    res.solvable=false;
    return res;
}