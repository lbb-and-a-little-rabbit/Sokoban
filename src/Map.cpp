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
    //level 9
    {
        "  ######",
        "  #    #",
        "###bbb #",
        "#p bxx #",
        "# bxxx##",
        "####  # ",
        "   #### "
    },
    //level 10
    {
        " ####  #####",
        "##  #  #   #",
        "# b ####b  #",
        "#  bxxxx b #",
        "##    # p ##",
        " ########## "
    },
    //level 11
    {
        "  ##### ",
        "###  p# ",
        "#  bx ##",
        "#  xbx #",
        "### Bb #",
        "  #   ##",
        "  ##### "
    },
    //level 12
    {
        "  ####  ",
        "  #xx#  ",
        " ## x## ",
        " #  bx# ",
        "## b  ##",
        "#  #bb #",
        "#  p   #",
        "########"
    },
    //level 13
    {
        "########",
        "#  #   #",
        "# bxxb #",
        "#pbxB ##",
        "# bxxb #",
        "#  #   #",
        "########"
    },
    //level 14
    {
        "  #### ",
        "  #  # ",
        "###b ##",
        "#  B p#",
        "#  B  #",
        "#  B ##",
        "###B # ",
        "  #x## ",
        "  ###  "
    },
    // level 15 - Microban 1 (David W. Skinner)
    {
        "####  ",
        "# x#  ",
        "#  ###",
        "#Bp  #",
        "#  b #",
        "#  ###",
        "####  "
    },
    // level 16 - Microban 2 (David W. Skinner)
    {
        "######",
        "#    #",
        "# #p #",
        "# bB #",
        "# xB #",
        "#    #",
        "######"
    },
    // level 17 - Microban 3 (David W. Skinner)
    {
        "  ####   ",
        "###  ####",
        "#     b #",
        "# #  #b #",
        "# x x#p #",
        "#########"
    },
    // level 18 - Microban 4 (David W. Skinner)
    {
        "########",
        "#      #",
        "# xBBbp#",
        "#      #",
        "#####  #",
        "    ####"
    },
    // level 19 - Microban 5 (David W. Skinner)
    {
        " #######",
        " #     #",
        " # xbx #",
        "## bpb #",
        "#  xbx #",
        "#      #",
        "########"
    },
    // level 20 - Microban 6 (David W. Skinner)
    {
        "###### #####",
        "#    ###   #",
        "# bb     #p#",
        "# b #xxx   #",
        "#   ########",
        "#####       "
    },
    // level 21 - Microban 7 (David W. Skinner)
    {
        "#######",
        "#     #",
        "# xbx #",
        "# bxb #",
        "# xbx #",
        "# bxb #",
        "#  p  #",
        "#######"
    },
    // level 22 - Microban 8 (David W. Skinner)
    {
        "  ######",
        "  # xxp#",
        "  # bb #",
        "  ## ###",
        "   # #  ",
        "   # #  ",
        "#### #  ",
        "#    ## ",
        "# #   # ",
        "#   # # ",
        "###   # ",
        "  ##### "
    },
    // level 23 - Microban 9 (David W. Skinner)
    {
        "##### ",
        "#x  ##",
        "#pbb #",
        "##   #",
        " ##  #",
        "  ##x#",
        "   ###"
    },
    // level 24 - Microban 10 (David W. Skinner)
    {
        "      #####",
        "      #x  #",
        "      #x# #",
        "#######x# #",
        "# p b b b #",
        "# # # # ###",
        "#       #  ",
        "#########  "
    },
    // level 25 - Microban 11 (David W. Skinner)
    {
        "  ###### ",
        "  #    # ",
        "  # ##p##",
        "### # b #",
        "# xx# b #",
        "#       #",
        "#  ######",
        "####     "
    },
    // level 26 - Microban 12 (David W. Skinner)
    {
        "#####    ",
        "#   ##   ",
        "# b  #   ",
        "## b ####",
        " ###px  #",
        "  #  x# #",
        "  #     #",
        "  #######"
    },
    // level 27 - Microban 13 (David W. Skinner)
    {
        "####   ",
        "#x ##  ",
        "#xp #  ",
        "#x b#  ",
        "##b ###",
        " # b  #",
        " #    #",
        " #  ###",
        " ####  "
    },
    // level 28 - Microban 14 (David W. Skinner)
    {
        "#######",
        "#     #",
        "# # # #",
        "#x bBp#",
        "#   ###",
        "#####  "
    },
    // level 29 - Microban 15 (David W. Skinner)
    {
        "     ### ",
        "######p##",
        "#    xB #",
        "#   #   #",
        "#####b# #",
        "    #   #",
        "    #####"
    },
    // level 30 - Microban 16 (David W. Skinner)
    {
        " ####     ",
        " #  ####  ",
        " #     ## ",
        "## ##   # ",
        "#x x# pb##",
        "#   # bb #",
        "#  x#    #",
        "##########"
    },
    // level 31 - Microban 17 (David W. Skinner)
    {
        "##### ",
        "# p # ",
        "#xxx# ",
        "#bbb##",
        "#    #",
        "#    #",
        "######"
    },
    // level 32 - Microban 18 (David W. Skinner)
    {
        "#######",
        "#     #",
        "#x x  #",
        "# ## ##",
        "#  b # ",
        "###b # ",
        "  #p # ",
        "  #  # ",
        "  #### "
    },
    // level 33 - Microban 19 (David W. Skinner)
    {
        "########",
        "#   xx #",
        "#  pbb #",
        "##### ##",
        "   #  # ",
        "   #  # ",
        "   #  # ",
        "   #### "
    },
    // level 34 - Microban 20 (David W. Skinner)
    {
        "#######  ",
        "#     ###",
        "#  pbbxx#",
        "#### ## #",
        "  #     #",
        "  #  ####",
        "  #  #   ",
        "  ####   "
    },
    //level 35 (previously 15), AI limit
    {
        "  ######## ",
        "###      # ",
        "#   bpb  # ",
        "#  b b b # ",
        "#####b#####",
        "   ## x xx#",
        "   #  x#  #",
        "   # x   x#",
        "   ####  ##",
        "      #### "
    },
    //level
    {
        "   #####   ",
        "  ##p  ##  ",
        " ## b b ## ",
        " #  #x#  # ",
        "## BxxxB ##",
        "#   # #   #",
        "# b B B b #",
        "####   ####",
        "   #####   "
    },
    //level
    {
        " ######### ",
        " #  ##   # ",
        " #   b   # ",
        " #b ### b# ",
        " # #xxx# # ",
        "## #xxx# ##",
        "# b  b  b #",
        "#     #p  #",
        "###########"
    },
    //level
    {
        "  ######  ",
        "  #    ###",
        "  # b    #",
        "### b ## #",
        "#xxx b   #",
        "#xxxb#b ##",
        "#### # b #",
        "   #  p  #",
        "   #######"
    },
    //level AI 
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
    //level 
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