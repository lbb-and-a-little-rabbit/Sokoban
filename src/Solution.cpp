#include "Solution.h"

Solution::Solution(int idx,Map &m){
    current_board.current_map=m.getMap(idx);

    bool find=false;
    for(int i=0;i<current_board.current_map.size();i++){
        if(find) break;
        for(int j=0;j<current_board.current_map[0].size();i++){
            if(current_board.current_map[i][j]=='p'||current_board.current_map[i][j]=='P'){
                current_board.player_position={i,j};
                find=true;
                break;
            }
        }
    }
}

Solution::Solution(std::vector<std::string> cur_map){
    current_board.current_map=cur_map;

    bool find=false;
    for(int i=0;i<current_board.current_map.size();i++){
        if(find) break;
        for(int j=0;j<current_board.current_map[0].size();i++){
            if(current_board.current_map[i][j]=='p'||current_board.current_map[i][j]=='P'){
                current_board.player_position={i,j};
                find=true;
                break;
            }
        }
    }
}

Result Solution::solve(int choice){
    if(choice==1){
        return UCS();
    }
    std::cerr << "Failed to find solution\n";
    exit(-1);
}
