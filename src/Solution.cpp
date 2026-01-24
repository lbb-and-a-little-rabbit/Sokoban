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
    std::cerr << "Failed to find solution\n";
    exit(-1);
}

Result Solution::UCS(){
    Result res;
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
