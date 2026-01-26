#include "LearningAgent.h"

Move LearningAgent::act(Board& state){
    std::string s = state.MaptoString();

    // 初始化
    if(!Q.count(s)){
        Q[s] = {0,0,0,0};
    }

    // 探索
    if((double)rand() / RAND_MAX < epsilon){
        auto moves = state.generateMoves();
        if(moves.empty()) return Move::None;
        return moves[rand() % moves.size()];
    }

    // 利用
    auto moves = state.generateMoves();
    if(moves.empty()) return Move::None;
    Move best = moves[0];
    double bestQ = -1e9;

    for(Move m : moves){
        int idx = (int)m;
        if(Q[s][idx] > bestQ){
            bestQ = Q[s][idx];
            best = m;
        }
    }

    return best;
}

void LearningAgent::observe(Board& s,
                            Move a,
                            int reward,
                            Board& s2,
                            bool done){
    if(a==Move::None) return;

    std::string s1 = s.MaptoString();
    std::string s2k = s2.MaptoString();

    if(!Q.count(s1)) Q[s1] = {0,0,0,0};
    if(!Q.count(s2k)) Q[s2k] = {0,0,0,0};

    int ai = (int)a;

    double maxQ2 = 0.0;
    if(!done){
        auto nextMoves = s2.generateMoves();
        if(!nextMoves.empty()){
            maxQ2 = -1e9;
            for(Move m : nextMoves){
                maxQ2 = std::max(maxQ2, Q[s2k][(int)m]);
            }
        }
    }

    Q[s1][ai] += alpha * (reward + gamma * maxQ2 - Q[s1][ai]);
}

void LearningAgent::decayEpsilon() {
    // epsilon 衰减到 0.01
    epsilon = std::max(0.01, epsilon * 0.995);
}

void LearningAgent::save(const std::string& path) {
    std::ofstream out(path);
    if(!out){
        std::cerr << "Failed to open " << path << " for saving Q table\n";
        return;
    }

    for(const auto& [state, arr] : Q){
        out << state << " ";
        for(int i=0;i<4;i++){
            out << arr[i] << " ";
        }
        out << "\n";
    }
    out.close();
}