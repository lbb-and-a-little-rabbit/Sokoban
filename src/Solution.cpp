#include "Solution.h"

Solution::Solution(int idx,Map &m){
    current_board.current_map=m.getMap(idx);
    current_board.Init_player_position();
}

Solution::Solution(std::vector<std::string> cur_map){
    current_board.current_map=cur_map;
    current_board.Init_player_position();
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
    //TODO

    return res;
}
