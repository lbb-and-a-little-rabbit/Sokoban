#include "SaveSystem.h"
#include <fstream>
#include <filesystem>

SaveData SaveSystem::load() {
    SaveData data;

    // 确保 save 目录存在
    std::filesystem::create_directory(SAVE_DIR);

    std::ifstream in(SAVE_FILE);
    if(!in.is_open()){
        // 第一次运行，没有存档
        return data;
    }

    std::string key;
    char eq;
    in >> key >> eq >> data.maxUnlockedLevel;
    return data;
}

void SaveSystem::save(const SaveData& data) {
    std::filesystem::create_directory(SAVE_DIR);

    std::ofstream out(SAVE_FILE);
    out << "maxUnlockedLevel = " << data.maxUnlockedLevel;
}
