#include "FileActivityRepository.h"
#include <filesystem>
#include <fstream>

namespace infrastructure::repositories {

FileActivityRepository::FileActivityRepository(std::string filePath)
    : filePath_(std::move(filePath))
{
}

std::expected<std::vector<domain::entities::Activity>, std::string>
FileActivityRepository::loadActivities() const
{
    if (!std::filesystem::exists(filePath_)) {
        return std::unexpected("File not found: " + filePath_);
    }

    std::ifstream file(filePath_);
    if (!file.is_open()) {
        return std::unexpected("Permission denied: " + filePath_);
    }

    std::vector<domain::entities::Activity> activities;
    std::string line;

    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty())
            continue;

        // Parse line: "ActivityName,Category"
        auto commaPos = line.find(',');
        if (commaPos == std::string::npos) {
            return std::unexpected("Invalid format in line: " + line);
        }

        std::string name = line.substr(0, commaPos);
        std::string categoryStr = line.substr(commaPos + 1);

        // Trim name and category
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);
        categoryStr.erase(0, categoryStr.find_first_not_of(" \t"));
        categoryStr.erase(categoryStr.find_last_not_of(" \t") + 1);

        auto category = domain::entities::Activity::stringToCategory(categoryStr);
        if (!category) {
            return std::unexpected("Invalid category: " + categoryStr);
        }

        activities.emplace_back(std::move(name), *category);
    }

    if (file.bad()) {
        return std::unexpected("Read error: " + filePath_);
    }

    return activities;
}

std::expected<void, std::string>
FileActivityRepository::saveActivities(const std::vector<domain::entities::Activity>& activities) const
{
    std::ofstream file(filePath_);
    if (!file.is_open()) {
        return std::unexpected("Permission denied: " + filePath_);
    }

    for (const auto& activity : activities) {
        file << activity.getName() << ","
             << activity.categoryToString(activity.getCategory()) << "\n";
    }

    if (file.bad()) {
        return std::unexpected("Write error: " + filePath_);
    }

    return {};
}

bool FileActivityRepository::isAvailable() const noexcept
{
    return std::filesystem::exists(filePath_) || std::filesystem::exists(std::filesystem::path(filePath_).parent_path());
}

std::string FileActivityRepository::getRepositoryInfo() const noexcept
{
    return "FileActivityRepository: " + filePath_;
}

} // namespace infrastructure::repositories

// Factory implementation
namespace domain::repositories {

std::unique_ptr<IActivityRepository> createFileActivityRepository(
    const std::string& filePath)
{
    return std::make_unique<infrastructure::repositories::FileActivityRepository>(filePath);
}

} // namespace domain::repositories
