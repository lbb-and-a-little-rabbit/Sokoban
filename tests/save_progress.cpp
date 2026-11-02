#include "SaveSystem.h"
#include "EncryptedPak.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    const auto root = std::filesystem::absolute("build/save-progress-test");
    std::filesystem::create_directories(root / "save");
    if(!PHYSFS_init(argv[0])) { std::cerr << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << "\n"; return 1; }
    assert(PHYSFS_mount(root.string().c_str(), nullptr, 0));
    assert(PHYSFS_setWriteDir(root.string().c_str()));
    auto check = [&](const std::string& text, int expected) {
        auto encrypted = EncryptBuffer(std::vector<char>(text.begin(), text.end()));
        {
            std::ofstream file(root / "save/save.gxyz", std::ios::binary);
            file.write(encrypted.data(), encrypted.size());
        }
        auto data = SaveSystem::load();
        assert(data.maxUnlockedLevel == expected);
        SaveSystem::save(data);
        assert(SaveSystem::load().maxUnlockedLevel == expected);
    };
    check("maxUnlockedLevel = 0", 0);
    check("maxUnlockedLevel = 14", 14);
    check("maxUnlockedLevel = 15", 15);
    check("maxUnlockedLevel = 20", 20);
    check("maxUnlockedLevel = 35\nlayoutVersion = 2", 15);
    check("maxUnlockedLevel = 40\nlayoutVersion = 2", 20);
    check("maxUnlockedLevel = 16\nlayoutVersion = 3", 16);
    check("maxUnlockedLevel = 35\nlayoutVersion = 3", 35);
    check("maxUnlockedLevel = 40\nlayoutVersion = 3", 40);
    check("maxUnlockedLevel = -1", 0);
    check("maxUnlockedLevel = 999\nlayoutVersion = 3", 40);
    check("invalid", 0);
    SaveData data;
    data.maxUnlockedLevel = 15;
    SaveSystem::save(data);
    data = SaveSystem::load();
    ++data.maxUnlockedLevel;
    SaveSystem::save(data);
    assert(SaveSystem::load().maxUnlockedLevel == 16);
    PHYSFS_deinit();
    std::cout << "Save migration, round trips, bounds and sequential unlock passed.\n";
}
