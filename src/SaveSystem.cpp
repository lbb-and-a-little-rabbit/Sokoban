#include "SaveSystem.h"
#include "EncryptedPak.h"
#include <physfs.h>
#include <sstream>

SaveData SaveSystem::load()
{
    SaveData data;

    if (!PHYSFS_exists(SAVE_FILE))
        return data;

    PHYSFS_File* file = PHYSFS_openRead(SAVE_FILE);
    if (!file)
        return data;

    PHYSFS_sint64 len = PHYSFS_fileLength(file);

    std::vector<char> buffer(len);
    PHYSFS_readBytes(file, buffer.data(), len);
    PHYSFS_close(file);

    // ===== 解密 =====
    buffer = DecryptBuffer(buffer);

    // 解析文本
    std::string content(buffer.begin(), buffer.end());
    std::istringstream iss(content);

    std::string key;
    char eq;
    iss >> key >> eq >> data.maxUnlockedLevel;

    return data;
}

void SaveSystem::save(const SaveData& data)
{
    PHYSFS_mkdir(SAVE_DIR);

    std::ostringstream oss;
    oss << "maxUnlockedLevel = " << data.maxUnlockedLevel;

    std::string text = oss.str();

    std::vector<char> buffer(text.begin(), text.end());

    // ===== 加密 =====
    buffer = EncryptBuffer(buffer);

    PHYSFS_File* file = PHYSFS_openWrite(SAVE_FILE);
    if (!file)
        return;

    PHYSFS_writeBytes(file, buffer.data(), buffer.size());
    PHYSFS_close(file);
}