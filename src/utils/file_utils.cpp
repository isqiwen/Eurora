#include "file_utils.h"

#include <algorithm>
#include <unordered_set>

#include <boost/nowide/utf8_codecvt.hpp>

#include "utils/logger/logger.h"

namespace eurora::utils {

std::vector<std::filesystem::path> FileUtils::GetAllFilesInDir(const std::string& dir_path, const std::vector<std::string>& suffix) {
    std::unordered_set<std::string> allowed_extensions(suffix.begin(), suffix.end());
    std::vector<std::filesystem::path> list_of_files;

    try {
        if (std::filesystem::exists(dir_path) && std::filesystem::is_directory(dir_path)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dir_path)) {
                if (!entry.is_directory() && (allowed_extensions.empty() || allowed_extensions.count(entry.path().extension().string()) > 0)) {
                    list_of_files.push_back(entry.path());
                }
            }
        }
    } catch (const std::system_error& e) {
        LOG_INFO(e.what());
    }

    return list_of_files;
}

void FileUtils::SetUTF8(std::fstream& input) {
    try {
        std::locale loc(std::locale(), new boost::nowide::utf8_codecvt<wchar_t>());
        input.imbue(loc);
    } catch (const std::runtime_error& e) {
        STREAM_ERROR() << "Failed to set UTF-8 locale: " << e.what();
        throw;  // Re-throw the exception to indicate failure
    }
}

std::string FileUtils::LoadAsString(const std::filesystem::path& filepath, bool enable_throw_exception) {
    if (!std::filesystem::exists(filepath)) {
        if (enable_throw_exception) {
            STREAM_ERROR() << "File not found: " << filepath.string();
            throw FileNotExist(filepath.string());
        } else {
            STREAM_WARN() << "File not found, returning empty string: " << filepath.string();
            return "";
        }
    }

    try {
        std::ifstream ifs(filepath, std::ios::binary);  // Open in binary mode for consistent behavior
        if (!ifs.is_open()) {
            STREAM_ERROR() << "Failed to open file: " << filepath.string();
            if (enable_throw_exception) {
                throw std::ios_base::failure("Failed to open file: " + filepath.string());
            } else {
                return "";
            }
        }

        std::ostringstream oss;
        oss << ifs.rdbuf();  // Stream content into the stringstream
        return oss.str();
    } catch (const std::exception& e) {
        STREAM_ERROR() << "Error reading file: " << filepath.string() << " - " << e.what();
        if (enable_throw_exception) {
            throw;  // Re-throw the exception for upstream handling
        } else {
            return "";
        }
    }
}

}  // namespace eurora::utils
