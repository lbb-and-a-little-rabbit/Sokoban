#pragma once

#include "LearningAgent.h"
#include "SokobanEnv.h"

#include <thread>
#include <atomic>
#include <iostream>

struct TrainingStatus {
    std::atomic<int> currentEpisode{0};
    std::atomic<int> solved{0};          // 已经通关的次数
    std::atomic<double> avgReward{0.0};  // 平均奖励
};

class Trainer {
public:
    LearningAgent agent;
    int totalEpisodes = 1000;
    TrainingStatus* status = nullptr;   // 指向共享状态
    std::atomic<bool> stopFlag{false};

    void trainAsync(TrainingStatus* s);

    void stop() { stopFlag = true; }

    void train(int episodes);
    void save(const std::string& path);
};
