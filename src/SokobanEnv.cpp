#include "SokobanEnv.h"

SokobanEnv::SokobanEnv(Board &b) : cur(b) {}

Board& SokobanEnv::getState(){
    return cur;
}

bool SokobanEnv::isDone(){
    return cur.isTerminal();
}

StepResult SokobanEnv::step(Move m){
    //m应为合法
    
    //
    Board next=cur.applyMove(m);
    int reward=-1;

    //原地踏步
    if(next.MaptoString() == cur.MaptoString()){
        reward -= 5;
    }

    //箱子→目标 曼哈顿距离变化
    int distBefore = cur.sumBoxToTargetDist();
    int distAfter  = next.sumBoxToTargetDist();
    reward += 2 * (distBefore - distAfter);

    //终局奖励
    if(next.isTerminal()) reward+=2000;

    //推进奖励
    if(next.boxOnTargetIncreased(cur)) reward+=50;
    else reward-=5;

    //死局惩罚
    if(next.isDead()) reward-=500;
    
    cur=next;
    return {cur,reward,cur.isTerminal()};
}

void SokobanEnv::reset(Board& b){
    cur=b;
}