#include "Solution.h"
#include <cassert>
#include <iostream>
#include <set>

// Independent push-space search: all reachable standing positions are equivalent.
bool solvableByPushes(Board initial) {
    std::queue<Board> pending;
    std::set<std::string> seen;
    pending.push(initial);
    while(!pending.empty()) {
        Board board = pending.front(); pending.pop();
        if(board.isTerminal()) return true;
        const int height = board.current_map.size(), width = board.current_map[0].size();
        auto [py, px] = board.player_position;
        board.current_map[py][px] = board.current_map[py][px] == 'P' ? 'x' : ' ';
        std::set<std::pair<int,int>> reachable{{py,px}};
        std::queue<std::pair<int,int>> flood;
        flood.push({py,px});
        while(!flood.empty()) {
            auto [y,x] = flood.front(); flood.pop();
            for(auto d : dir) {
                int yy=y+d[0], xx=x+d[1];
                if(yy<0 || yy>=height || xx<0 || xx>=width) continue;
                char c=board.current_map[yy][xx];
                if((c==' ' || c=='x') && reachable.insert({yy,xx}).second)
                    flood.push({yy,xx});
            }
        }
        auto anchor=*reachable.begin();
        auto key=board.MaptoString()+std::to_string(anchor.first)+","+std::to_string(anchor.second);
        if(!seen.insert(key).second) continue;
        for(auto [y,x] : reachable) {
            Board standing=board;
            standing.player_position={y,x};
            standing.current_map[y][x]=board.current_map[y][x]=='x' ? 'P' : 'p';
            for(auto move : standing.generateMoves()) {
                char ahead=standing.current_map[y+dir[move][0]][x+dir[move][1]];
                if(ahead=='b' || ahead=='B') pending.push(standing.applyMove(move));
            }
        }
    }
    return false;
}
int main() {
    std::cout << std::unitbuf;
    // Exhausting an impossible board must be distinct from a time limit.
    const auto impossible = Solution(std::vector<std::string>{
        "#####", "#b p#", "#  x#", "#####"
    }).solve(2);
    assert(!impossible.solvable && !impossible.timeout);
    std::cout << "Unsolvable board correctly distinguished from timeout\n";
    Map maps;
    assert(maps.getLevelCount() == 100);
    for (int i = 15; i < maps.getLevelCount(); ++i) {
        if(i >= 35 && i <= 40) continue; // Existing original levels.
        auto rows = maps.getMap(i);
        Board board;
        board.current_map = rows;
        int players = 0, boxes = 0, targets = 0;
        for (int y = 0; y < rows.size(); ++y) {
            assert(rows[y].size() == rows[0].size());
            for (int x = 0; x < rows[y].size(); ++x) {
                char c = rows[y][x];
                assert(std::string(" #xbBpP").find(c) != std::string::npos);
                if(c == 'p' || c == 'P') { ++players; board.player_position = {y, x}; }
                if(c == 'b' || c == 'B') ++boxes;
                if(c == 'x' || c == 'B' || c == 'P') ++targets;
            }
        }
        assert(players == 1 && boxes > 0 && boxes == targets);
        if(i >= 41) {
            std::cout << "Push search level " << i << std::endl;
            assert(solvableByPushes(board));
            std::cout << "Level " << i << ": verified solvable\n";
            continue;
        }
        auto result = Solution(rows).solve(2);
        std::cout << "Checking " << i << " timeout=" << result.timeout << " solvable=" << result.solvable << "\n";
        if(result.timeout) {
            assert(solvableByPushes(board));
            std::cout << "Level " << i << ": verified by independent push search\n";
            continue;
        }
        assert(result.solvable);
        for(auto move : result.path) {
            assert(board.canMove(move));
            board = board.applyMove(move);
        }
        assert(board.isTerminal());
        std::cout << "Level " << i << ": " << result.path.size() << " legal steps, solved\n";
    }
}
