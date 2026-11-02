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
    },
    // level 41 - Microban 21 (David W. Skinner)
    {
        "####   ",
        "#  ####",
        "# x x #",
        "# bb#p#",
        "##    #",
        " ######"
    },
    // level 42 - Microban 22 (David W. Skinner)
    {
        "#####  ",
        "#   ###",
        "#x x  #",
        "#   # #",
        "## #  #",
        " #pbb #",
        " #    #",
        " #  ###",
        " ####  "
    },
    // level 43 - Microban 23 (David W. Skinner)
    {
        "#######",
        "#  B  #",
        "#     #",
        "## # ##",
        " #bpx# ",
        " #   # ",
        " ##### "
    },
    // level 44 - Microban 24 (David W. Skinner)
    {
        "# #####",
        "  #   #",
        "###bbp#",
        "#   ###",
        "#     #",
        "# x x #",
        "#######"
    },
    // level 45 - Microban 25 (David W. Skinner)
    {
        " ####  ",
        " #  ###",
        " # bb #",
        "##xxx #",
        "#  pb #",
        "#   ###",
        "#####  "
    },
    // level 46 - Microban 26 (David W. Skinner)
    {
        " #####",
        " # p #",
        " #   #",
        "###b #",
        "# xxx#",
        "# bb #",
        "###  #",
        "  ####"
    },
    // level 47 - Microban 27 (David W. Skinner)
    {
        "###### ",
        "#   x# ",
        "# ## ##",
        "#  bbp#",
        "# #   #",
        "#x  ###",
        "#####  "
    },
    // level 48 - Microban 28 (David W. Skinner)
    {
        "#####  ",
        "#   #  ",
        "# p #  ",
        "# bb###",
        "##x x #",
        " #    #",
        " ######"
    },
    // level 49 - Microban 29 (David W. Skinner)
    {
        "     ##### ",
        "     #   ##",
        "     #    #",
        " ######   #",
        "##     #x #",
        "# b b p  ##",
        "# ######x# ",
        "#        # ",
        "########## "
    },
    // level 50 - Microban 30 (David W. Skinner)
    {
        "####  ",
        "#  ###",
        "# bb #",
        "#xxx #",
        "# pb #",
        "#   ##",
        "##### "
    },
    // level 51 - Microban 31 (David W. Skinner)
    {
        "  #### ",
        " ##  # ",
        "##pbx##",
        "# bb  #",
        "# x x #",
        "###   #",
        "  #####"
    },
    // level 52 - Microban 32 (David W. Skinner)
    {
        " ####  ",
        "##  ###",
        "#     #",
        "#xBBbp#",
        "#   ###",
        "##  #  ",
        " ####  "
    },
    // level 53 - Microban 33 (David W. Skinner)
    {
        "#######",
        "#x #  #",
        "#  b  #",
        "#x b#p#",
        "#  b  #",
        "#x #  #",
        "#######"
    },
    // level 54 - Microban 34 (David W. Skinner)
    {
        "  ####   ",
        "###  ####",
        "#       #",
        "#pbBBBx #",
        "#       #",
        "#########"
    },
    // level 55 - Microban 35 (David W. Skinner)
    {
        "  #### ",
        " ##  # ",
        " #x b# ",
        " #xb # ",
        " #xb # ",
        " #xb # ",
        " #x b##",
        " #   p#",
        " ##   #",
        "  #####"
    },
    // level 56 - Microban 36 (David W. Skinner)
    {
        "####           ",
        "#  ############",
        "# b b b b b p #",
        "# xxxxx       #",
        "###############"
    },
    // level 57 - Microban 37 (David W. Skinner)
    {
        "      ###",
        "##### #x#",
        "#   ###x#",
        "#   b #x#",
        "# b  b  #",
        "#####p# #",
        "    #   #",
        "    #####"
    },
    // level 58 - Microban 38 (David W. Skinner)
    {
        "##########",
        "#        #",
        "# ##x### #",
        "# # bb x #",
        "# x pb## #",
        "#####    #",
        "    ######"
    },
    // level 59 - Microban 39 (David W. Skinner)
    {
        "#####     ",
        "#   ####  ",
        "# # # x#  ",
        "#    b ###",
        "### #bx  #",
        "#   #p   #",
        "# # ######",
        "#   #     ",
        "#####     "
    },
    // level 60 - Microban 40 (David W. Skinner)
    {
        " ##### ",
        " #   # ",
        "##   ##",
        "# bbb #",
        "# xPx #",
        "#######"
    },
    // level 61 - Microban 41 (David W. Skinner)
    {
        "####### ",
        "#     # ",
        "#pbbb ##",
        "#  #xxx#",
        "##    ##",
        " ###### "
    },
    // level 62 - Microban 42 (David W. Skinner)
    {
        "   ####",
        "   #  #",
        "   #p #",
        "####bx#",
        "#   bx#",
        "# # bx#",
        "#    ##",
        "###### "
    },
    // level 63 - Microban 43 (David W. Skinner)
    {
        "     ####",
        "     # p#",
        "     #  #",
        "###### x#",
        "#   b  x#",
        "#  bb# x#",
        "#    ####",
        "###  #   ",
        "  ####   "
    },
    // level 64 - Microban 44 (David W. Skinner)
    {
        "#####",
        "#pbx#",
        "#####"
    },
    // level 65 - Microban 45 (David W. Skinner)
    {
        "######",
        "#xxx #",
        "#  b #",
        "# #b##",
        "#  b #",
        "#  p #",
        "######"
    },
    // level 66 - Microban 46 (David W. Skinner)
    {
        " ######",
        "##    #",
        "#  ## #",
        "# # b #",
        "#  B x#",
        "## #p##",
        " #   # ",
        " ##### "
    },
    // level 67 - Microban 47 (David W. Skinner)
    {
        "  #######  ",
        "###     #  ",
        "# b b   #  ",
        "# ### #####",
        "# p x x   #",
        "#   ###   #",
        "##### #####"
    },
    // level 68 - Microban 48 (David W. Skinner)
    {
        "######  ",
        "#  p #  ",
        "#  # ## ",
        "# x#  ##",
        "# xbbb #",
        "# x#   #",
        "####   #",
        "   #####"
    },
    // level 69 - Microban 49 (David W. Skinner)
    {
        "######  ",
        "# p  #  ",
        "# b# #  ",
        "# b  #  ",
        "# b ##  ",
        "### ####",
        " #  #  #",
        " #xxx  #",
        " #     #",
        " #######"
    },
    // level 70 - Microban 50 (David W. Skinner)
    {
        "  ####    ",
        "###  #####",
        "#  b  pxx#",
        "# b    # #",
        "### #### #",
        "  #      #",
        "  ########"
    },
    // level 71 - Microban 51 (David W. Skinner)
    {
        "####    ",
        "#  ###  ",
        "#    ###",
        "#  bBp #",
        "### x# #",
        "  #    #",
        "  ######"
    },
    // level 72 - Microban 52 (David W. Skinner)
    {
        "  ####",
        "### p#",
        "#  b #",
        "#  Bx#",
        "#  Bx#",
        "#  b #",
        "###  #",
        "  ####"
    },
    // level 73 - Microban 53 (David W. Skinner)
    {
        " ##### ",
        "##x x##",
        "# B B #",
        "#  #  #",
        "# b b #",
        "## p ##",
        " ##### "
    },
    // level 74 - Microban 54 (David W. Skinner)
    {
        "      ######",
        "      #    #",
        "  ##### x  #",
        "###  ###x  #",
        "# b  b  x ##",
        "# pbb # x # ",
        "##    ##### ",
        " ######     "
    },
    // level 75 - Microban 55 (David W. Skinner)
    {
        "########  ",
        "# p #  #  ",
        "#      #  ",
        "#####b #  ",
        "    #  ###",
        " ## #b xx#",
        " ## #  ###",
        "    ####  "
    },
    // level 76 - Microban 56 (David W. Skinner)
    {
        "#####  ",
        "#   ###",
        "#  b  #",
        "##B x #",
        " #   p#",
        " ######"
    },
    // level 77 - Microban 57 (David W. Skinner)
    {
        "  ####  ",
        "  #  #  ",
        "  #p #  ",
        "  #  #  ",
        "### ####",
        "#    B #",
        "#  b   #",
        "#####x #",
        "    ####"
    },
    // level 78 - Microban 58 (David W. Skinner)
    {
        "####   ",
        "#  ####",
        "#xBb  #",
        "# xb# #",
        "## p  #",
        " #   ##",
        " ##### "
    },
    // level 79 - Microban 59 (David W. Skinner)
    {
        "############ ",
        "#          # ",
        "# ####### p##",
        "# #         #",
        "# #  b   #  #",
        "# bb #####  #",
        "###  # # xxx#",
        "  #### #    #",
        "       ######"
    },
    // level 80 - Microban 60 (David W. Skinner)
    {
        " #########",
        " #       #",
        "##p##### #",
        "#  #   # #",
        "#  #   bx#",
        "#  ##b##x#",
        "##b##  #x#",
        "#   b  #x#",
        "#   #  ###",
        "########  "
    },
    // level 81 - Microban 61 (David W. Skinner)
    {
        "######## ",
        "#      # ",
        "# #### # ",
        "# #xxxp# ",
        "# ###b###",
        "# #     #",
        "#  bb b #",
        "####   ##",
        "   #x### ",
        "   ###   "
    },
    // level 82 - Microban 62 (David W. Skinner)
    {
        "   ##########",
        "####    ##  #",
        "#  bbbxxxxbp#",
        "#      ###  #",
        "#   #### ####",
        "#####        "
    },
    // level 83 - Microban 63 (David W. Skinner)
    {
        "#####   ####       ",
        "#   ##### x#       ",
        "#       b  ########",
        "###  #### xb    p #",
        "  #  #  #  ####   #",
        "  ####  ####  #####"
    },
    // level 84 - Microban 64 (David W. Skinner)
    {
        " ######   ",
        "##    #   ",
        "#   b #   ",
        "#  bb #   ",
        "### x#####",
        "  ##x# p #",
        "   #x  b #",
        "   #x ####",
        "   ####   "
    },
    // level 85 - Microban 65 (David W. Skinner)
    {
        "  ###### ",
        "  #    # ",
        "  #  b # ",
        " ####b # ",
        "## b b # ",
        "#xxxx# ##",
        "#     p #",
        "##  #   #",
        " ########"
    },
    // level 86 - Microban 66 (David W. Skinner)
    {
        "   ###   ",
        "   #p#   ",
        " ###b### ",
        "##  x  ##",
        "#  # #  #",
        "# #   # #",
        "# #   # #",
        "# #   # #",
        "#  # #  #",
        "## b b ##",
        " ##x x## ",
        "  #   #  ",
        "  #   #  ",
        "  #####  "
    },
    // level 87 - Microban 67 (David W. Skinner)
    {
        "#####  ",
        "#   ## ",
        "# #  # ",
        "#pbBx##",
        "##  x #",
        " # b# #",
        " ##   #",
        "  #####"
    },
    // level 88 - Microban 68 (David W. Skinner)
    {
        " ####     ",
        " #  ######",
        "##    b  #",
        "# x# b   #",
        "# x#b#####",
        "# xp #    ",
        "######    "
    },
    // level 89 - Microban 69 (David W. Skinner)
    {
        "####  #### ",
        "#  ####  # ",
        "#  #  #  # ",
        "#  #    b##",
        "#  x x#b  #",
        "#p ## # b #",
        "#   x #   #",
        "###########"
    },
    // level 90 - Microban 70 (David W. Skinner)
    {
        "#####   ",
        "# p ####",
        "#      #",
        "# b bb #",
        "##b##  #",
        "#   ####",
        "# xx  # ",
        "##xx  # ",
        " ###  # ",
        "   #### "
    },
    // level 91 - Microban 71 (David W. Skinner)
    {
        "###########  ",
        "#     #   ###",
        "# bpb # x  x#",
        "# ## ### ## #",
        "# #       # #",
        "# #   #   # #",
        "# ######### #",
        "#           #",
        "#############"
    },
    // level 92 - Microban 72 (David W. Skinner)
    {
        "  ####    ",
        " ##  #####",
        " #  b  p #",
        " #  b#   #",
        "#### #####",
        "#  #   #  ",
        "#    b #  ",
        "# xx#  #  ",
        "#  x####  ",
        "#  ##     ",
        "####      "
    },
    // level 93 - Microban 73 (David W. Skinner)
    {
        "####    ",
        "#  #####",
        "# bb b #",
        "#      #",
        "## ## ##",
        "#xxx#p# ",
        "# ### ##",
        "#      #",
        "#  #   #",
        "########"
    },
    // level 94 - Microban 74 (David W. Skinner)
    {
        " ####      ",
        " #  #######",
        " #b p#   x#",
        "## #bb   x#",
        "#  b  ##xx#",
        "#   # #####",
        "###   #    ",
        "  #####    "
    },
    // level 95 - Microban 75 (David W. Skinner)
    {
        " #######  ",
        "## xxxx## ",
        "#   ######",
        "#   b b p#",
        "###  b b #",
        "  ###    #",
        "    ######"
    },
    // level 96 - Microban 76 (David W. Skinner)
    {
        " #####    ",
        "##   #    ",
        "#    #####",
        "#  #x#   #",
        "#p #x# b #",
        "#  #x#  ##",
        "#    #  # ",
        "##  ##bb# ",
        " ##     # ",
        "  #  #### ",
        "  ####    "
    },
    // level 97 - Microban 77 (David W. Skinner)
    {
        "########## ",
        "# p xxxx # ",
        "#   ####b##",
        "## #  b b #",
        " # b      #",
        " #   ######",
        " #####     "
    },
    // level 98 - Microban 78 (David W. Skinner)
    {
        " #######   ",
        "##     ##  ",
        "#  b b  #  ",
        "# b b b #  ",
        "## ### ####",
        " #p  xxxxx#",
        " ##     ###",
        "  #######  "
    },
    // level 99 - Microban 79 (David W. Skinner)
    {
        " #########",
        " #    #  #",
        "## b#b#  #",
        "#  xbxp  #",
        "#  x#    #",
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