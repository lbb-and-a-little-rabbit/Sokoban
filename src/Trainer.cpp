#include "Trainer.h"
#include <cstdlib>
#include <ctime>

Board randomTrainingMap(){
    Map temp_map;
    int idx = rand() % temp_map.getLevelCount();
    Board b;
    b.current_map=temp_map.getMap(idx);
    bool find=false;
    for(int i=0;i<b.current_map.size();i++){
        if(find) break;
        for(int j=0;j<b.current_map[0].size();j++){
            if(b.current_map[i][j]=='p'||b.current_map[i][j]=='P'){
                b.player_position={i,j};
                find=true;
                break;
            }
        }
    }
    return b;
}

void Trainer::trainAsync(TrainingStatus* s){
        status = s;
        stopFlag = false;

        std::thread([this](){
            int solvedCount = 0;
            double rewardSum = 0;

            for(int ep = 0; ep < totalEpisodes && !stopFlag; ep++){
                if(status) status->currentEpisode = ep+1;

                Board init = randomTrainingMap();
                SokobanEnv env(init);

                int step = 0;
                const int MAX_STEP = 1000;
                int epReward = 0;

                while(step < MAX_STEP && !env.isDone()){
                    Board cur = env.getState();
                    Move a = agent.act(cur);

                    StepResult res = env.step(a);
                    epReward += res.reward;

                    agent.observe(cur, a, res.reward, res.next, res.done);

                    step++;
                }

                rewardSum += epReward;
                if(env.isDone()) solvedCount++;

                if(status){
                    status->solved = solvedCount;
                    status->avgReward = rewardSum / (ep+1);
                }

                agent.decayEpsilon();
            }

            std::cout << "Training finished!\n";
            agent.save("qtable.bin");

        }).detach();
}

void Trainer::train(int episodes){
    LearningAgent agent;

    for(int ep=0; ep<episodes; ep++){
        Board init = randomTrainingMap();
        SokobanEnv env(init);

        int step=0;
        while(!env.isDone() && step<300){
            Board s = env.getState();
            Move a = agent.act(s);
            auto [s2, r, done] = env.step(a);
            agent.observe(s,a,r,s2,done);
            step++;
        }

        agent.decayEpsilon();
    }

    agent.save("qtable.bin");
}
