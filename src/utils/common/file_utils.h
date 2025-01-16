#ifndef EURORA_UTILS_FILE_UTILS_H_
#define EURORA_UTILS_FILE_UTILS_H_

#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "utils/exception/exception.h"

namespace eurora::utils {

EURORA_MAKE_SIMPLE_EXCEPTION(FileNotExist);

class FileUtils {
public:
    static void SetUTF8(std::fstream& input);

    static std::vector<std::filesystem::path> GetAllFilesInDir(const std::string& dir_path, const std::vector<std::string>& suffix = {});

    static std::string LoadAsString(const std::filesystem::path& filepath, bool enable_throw_exception = true);
};

}  // namespace eurora::utils

#endif EURORA_UTILS_FILE_UTILS_H_
