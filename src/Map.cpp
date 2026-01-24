#include "Map.h"

std::vector<std::vector<std::string>> Map::maps={
    //level 0
    {
        "#####",
        "#   #",
        "# pb#",
        "#  x#",
        "#####"
    },
    //level 1
    {
        "######",
        "#    #",
        "# ## #",
        "# pbx#",
        "#    #",
        "######"
    },
    //level 2
    {
        "#######",
        "#  x x#",
        "#  b b#",
        "#   p #",
        "#     #",
        "#######"
    }
};

Map::Map() {}

int Map::getLevelCount(){
    return maps.size();
}

std::vector<std::string> Map::getMap(int idx){
    if(idx<0||idx>=getLevelCount()){
        std::cerr << "Failed to load Map!\n";
        exit(-1);
    }
    return maps[idx];
}