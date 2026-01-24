#pragma once

#include <iostream>
#include <vector>
#include <string>

class Map{
    static std::vector<std::vector<std::string>> maps;

public:
    Map();

    int getLevelCount();
    std::vector<std::string> getMap(int idx);
};