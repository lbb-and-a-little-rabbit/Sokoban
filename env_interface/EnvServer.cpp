#include "EnvServer.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Map.h"           // 提供 Map, getLevelCount(), getMap(idx)
#include "Board.h"         // 提供 Board 类型与 Board::MaptoString()
#include "SokobanEnv.h"    // 提供 SokobanEnv, StepResult, Move

// 将 Board::MaptoString() 的多行 map 变成单行表示，行用 '|' 连接
std::string encodeBoardLine(const std::string &mapstr) {
    std::string out;
    out.reserve(mapstr.size());
    for (char c : mapstr) {
        if (c == '\n' || c == '\r') {
            out.push_back('|');
        } else {
            // 避免空格连续导致解析问题：保留空格作为地面
            out.push_back(c);
        }
    }
    // trim leading/trailing '|' 可能出现的多余
    while (!out.empty() && out.front() == '|') out.erase(out.begin());
    while (!out.empty() && out.back() == '|') out.pop_back();
    return out;
}

int main(int argc, char** argv) {
    std::srand((unsigned)std::time(nullptr));

    int fixed_level = 0; // -1 表示随机，根据需要修改

    if (argc >= 2) {
        std::string arg = argv[1];

        // 情况 1：直接给数字
        if (std::isdigit(arg[0])) {
            fixed_level = std::stoi(arg);
        }
        // 情况 2：levelX
        else if (arg.rfind("level", 0) == 0) {
            fixed_level = std::stoi(arg.substr(5));
        }
    }

    // 将 Map 视为关卡集合，从中随机选择
    Map maps; // 假设 Map 有默认构造并加载所有关卡
    int nlevels = maps.getLevelCount();
    if (nlevels <= 0) {
        std::cerr << "No levels found in Map!\n";
        return 1;
    }

    auto pickBoard = [&](Board &b){
        int idx;

        if (fixed_level >= 0 && fixed_level < nlevels) {
            idx = fixed_level;
        } else {
            idx = std::rand() % nlevels;
        }

        b.current_map = maps.getMap(idx);

        // 找玩家位置
        for (int i = 0; i < (int)b.current_map.size(); ++i) {
            for (int j = 0; j < (int)b.current_map[i].size(); ++j) {
                if (b.current_map[i][j] == 'p' || b.current_map[i][j] == 'P' ||
                    b.current_map[i][j] == '@') {
                    b.player_position = {i, j};
                    return;
                }
            }
        }
    };

    // 初始化 Board + Env
    Board board;
    pickBoard(board);
    SokobanEnv env(board);

    // Output initial state line (state reward done)
    std::string initial_map = env.getState().MaptoString();
    std::string enc = encodeBoardLine(initial_map);
    // initial reward 0, not done
    std::cout << enc << "\t" << 0.0 << "\t" << 0 << std::endl;
    std::cout.flush();

    // 主循环：阻塞读取 action（整数），执行 step，输出 next_state reward done
    while (true) {
        int action;
        if (!(std::cin >> action)) {
            // EOF or pipe closed -> exit cleanly
            break;
        }



        // 执行动作
        StepResult res = env.step(static_cast<Move>(action));

        // 输出 next state
        std::string next_map = env.getState().MaptoString();
        std::string next_enc = encodeBoardLine(next_map);

        double reward = res.reward;
        int done = res.done ? 1 : 0;

        std::cout << next_enc << "\t" << reward << "\t" << done << std::endl;
        std::cout.flush();

        // 如果回合结束（done），重置新关并立刻输出新关的初始状态（供 Python 作为下一轮的 state）
        if (res.done) {
            Board nb;
            pickBoard(nb);
            env.reset(nb); // 使用 SokobanEnv::reset(Board&)
            std::string new_map = env.getState().MaptoString();
            std::string new_enc = encodeBoardLine(new_map);
            std::cout << new_enc << "\t" << 0.0 << "\t" << 0 << std::endl;
            std::cout.flush();
        }
    }

    return 0;
}
