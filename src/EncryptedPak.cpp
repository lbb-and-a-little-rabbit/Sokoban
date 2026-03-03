#include "EncryptedPak.h"
#include <physfs.h>
#include <fstream>
#include <iostream>
#include <filesystem>

static std::vector<char> gPakData;  // 必须全局，不能是局部
static constexpr unsigned char SAVE_KEY = 
    'S' ^ 'o' ^ 'k' ^ 'o' ^ 'b' ^ 'a' ^ 'n';

static void xorBuffer(std::vector<char>& data, unsigned char key){
    for (auto& b : data)
        b ^= key;
}

bool MountEncryptedPak(const std::string& filename){
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Failed to open encrypted pak\n";
        return false;
    }

    gPakData.assign(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    );

    // 解密
    xorBuffer(gPakData, 's' ^ 'n' ^ 'l' ^ 'b' ^ 'c');   // key

    if (!PHYSFS_mountMemory(
        gPakData.data(),
        gPakData.size(),
        nullptr,
        "data.pak",
        "/",
        1))
    {
        std::cout << "Mount memory pak failed\n";
        return false;
    }

    return true;
}

// ====== 加密 ======
std::vector<char> EncryptBuffer(const std::vector<char>& data){
    std::vector<char> result = data;
    xorBuffer(result, SAVE_KEY);
    return result;
}

// ====== 解密 ======
std::vector<char> DecryptBuffer(const std::vector<char>& data){
    std::vector<char> result = data;
    xorBuffer(result, SAVE_KEY);
    return result;
}

std::string DecryptToTempFile(const std::string& virtualPath){
    PHYSFS_File* file = PHYSFS_openRead(virtualPath.c_str());
    if (!file)
        return "";

    PHYSFS_sint64 len = PHYSFS_fileLength(file);

    std::vector<char> buffer(len);
    PHYSFS_readBytes(file, buffer.data(), len);
    PHYSFS_close(file);

    // 生成文件名
    std::filesystem::path p(virtualPath);
    std::string filename = p.filename().string();
    std::string tempPath = "temp/" + filename;

    // 如果已经存在就不重复写
    if (!std::filesystem::exists(tempPath))
    {
        std::ofstream out(tempPath, std::ios::binary);
        out.write(buffer.data(), buffer.size());
        out.close();
    }

    return tempPath;
}