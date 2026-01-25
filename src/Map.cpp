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
        "# pb #",
        "#   x#",
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
        " #######  ",
        " #     ###",
        "##b###   #",
        "#p  b  b #",
        "# xx# b ##",
        "##xx#   # ",
        " ######## "
    },
    //level 6
    {
        " #### ",
        "##  # ",
        "# pb# ",
        "##b ##",
        "## b #",
        "#xb  #",
        "#xxBx#",
        "######",
    },
    //level 7
    {
        " ####   ",
        " #p ### ",
        " # b  # ",
        "### # ##",
        "#x# #  #",
        "#xb  # #",
        "#x   b #",
        "########",
    },
    //level 8
    {
        "   #######",
        "  ##  # p#",
        "  #   #  #",
        "  #b b b #",
        "  # b##  #",
        "### b # ##",
        "#xxxxx  # ",
        "######### ",
    },
    //level 9 you_ge_diao
    {  
        "  #####   ",
        "  # xx#   ",
        "  # bx#   ",
        "  #  x#   ",
        "###xbx####",
        "# b  b  p#",
        "# ### ## #",
        "# b    b #",
        "#   #    #",
        "##########",

    },
    //level 10
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