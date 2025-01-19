#pragma once

#include <filesystem>

namespace eurora::core {

sdt::filesystem::path DefaultWorkingFolder();
sdt::filesystem::path DefaultEuroraHome();
sdt::filesystem::path DefaultDatabaseFolder();
sdt::filesystem::path DefaultStorageFolder();

}  // namespace eurora::core
