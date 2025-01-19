#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace eurora::fe {

class TaskManager {
public:
    struct Task {
        std::string id;
        std::string status;
        std::vector<std::string> input_files;
        std::string output_file;
    };

    std::string createTask(const std::vector<std::string>& input_files, const std::string& output_file);
    std::optional<Task> getTask(const std::string& id);
    void updateTaskStatus(const std::string& id, const std::string& status);

    std::vector<Task> listTasks() const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, Task> tasks_;
};
}  // namespace eurora::fe
