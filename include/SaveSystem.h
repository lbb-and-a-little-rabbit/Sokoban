#pragma once
#include <string>

#include "physfs_assistant.h"

struct SaveData {
    int maxUnlockedLevel = 0;
};

class SaveSystem {
public:
    static SaveData load();
    static void save(const SaveData& data);

private:
    static constexpr const char* SAVE_DIR = "save";
    static constexpr const char* SAVE_FILE = "save/save.gxyz";
};
