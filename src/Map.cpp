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
    },
    //level 3
    {
        "  ###   ",
        "  #x#   ",
        "  # ####",
        "###b bx#",
        "#x bp###",
        "####b#  ",
        "   #x#  ",
        "   ###  "
    },
    //level 4
    {
        "#####    ",
        "#p  #    ",
        "# bb# ###",
        "# b # #x#",
        "### ###x#",
        " ##    x#",
        " #   #  #",
        " #   ####",
        " #####   "
    },
    //level 5
    {
        "##########",
        "#  ##    #",
        "# bxxb   #",
        "# bxxb   #",
        "#pbxxb   #",
        "# bxxb   #",
        "# bxxb   #",
        "#  ##    #",
        "##########"
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