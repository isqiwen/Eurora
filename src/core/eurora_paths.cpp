#pragma once

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "eurora/utils/logger.h"

namespace eurora::core {

namespace fs = std::filesystem;

namespace {

std::string GetExecutablePath() {
#if defined(_WIN32) || defined(_WIN64)
    char buffer[MAX_PATH];
    if (GetModuleFileNameA(NULL, buffer, MAX_PATH) == 0) {
        throw std::runtime_error("Could not determine the location of the Gadgetron binary.");
    }
    return std::string(buffer);

#elif defined(__APPLE__)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        throw std::runtime_error("Could not determine the location of the Gadgetron binary.");
    }
    return fs::canonical(path).string();

#elif defined(__linux__)
    std::string path;
    char buffer[4096];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len == -1) {
        throw std::runtime_error("Could not determine the location of the Gadgetron binary.");
    }
    buffer[len] = '\0';
    path        = buffer;
    return fs::canonical(path).string();
#else
    throw std::runtime_error("Unsupported platform for determining the executable path.");
#endif
}

fs::path GetDataDirectory() {
#if defined(_WIN32)
    const char* appdata = std::getenv("APPDATA");
    if (!appdata) {
        throw std::runtime_error("Could not determine the APPDATA directory.");
    }
    return fs::path(appdata) / "eurora";
#else
    const char* home = std::getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        if (!pw) {
            throw std::runtime_error("Could not determine the home directory.");
        }
        home = pw->pw_dir;
    }
    return fs::path(home) / ".eurora";
#endif
}

}  // namespace

fs::path DefaultWorkingFolder() {
#if defined(_WIN32)
    return "C:/temp/eurora/";
#else
    return "/tmp/eurora/";
#endif
}

fs::path DefaultEuroraHome() {
    if (const char* home = std::getenv("EURORA_HOME")) {
        return fs::path(home);
    }

    fs::path executable_path = GetExecutablePath();
    fs::path eurora_home     = executable_path.parent_path().parent_path();

    STREAM_INFO() << "Executable path: " << executable_path << std::endl;
    STREAM_INFO() << "Default Eurora home: " << eurora_home << std::endl;

    return eurora_home;
}

fs::path DefaultStorageFolder() { return GetDataDirectory() / "storage"; }

fs::path DefaultDatabaseFolder() { return DefaultStorageFolder() / "database"; }

}  // namespace eurora::core
