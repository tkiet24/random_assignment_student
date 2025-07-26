#pragma once

#include "../../domain/repositories/IActivityRepository.h"
#include "../../infrastructure/utils/Results.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace infrastructure::repositories {

// File-based Activity Repository implementation
class FileActivityRepository : public domain::repositories::IActivityRepository {
private:
    std::string filePath_;

public:
    explicit FileActivityRepository(std::string filePath)
        : filePath_(std::move(filePath))
    {
    }

    // Load activities từ file
    [[nodiscard]] infrastructure::utils::Result<std::vector<domain::entities::Activity>>
    loadActivities() const override
    {

        if (!std::filesystem::exists(filePath_)) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::NotFound));
        }

        std::ifstream file(filePath_);
        if (!file.is_open()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::PermissionDenied));
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
                return infrastructure::utils::unexpected(utils::makeValidationError(utils::ValidationError::FormatError));
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
                return infrastructure::utils::unexpected(utils::makeValidationError(utils::ValidationError::InvalidData));
            }

            activities.emplace_back(std::move(name), *category);
        }

        if (file.bad()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::ReadError));
        }

        return activities;
    }

    // Save activities to file
    [[nodiscard]] infrastructure::utils::Result<void>
    saveActivities(const std::vector<domain::entities::Activity>& activities) const override
    {

        std::ofstream file(filePath_);
        if (!file.is_open()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::PermissionDenied));
        }

        for (const auto& activity : activities) {
            file << activity.getName() << ","
                 << activity.categoryToString(activity.getCategory()) << "\n";
        }

        if (file.bad()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::ReadError));
        }

        return {};
    }

    [[nodiscard]] bool isAvailable() const noexcept override
    {
        return std::filesystem::exists(filePath_) || std::filesystem::exists(std::filesystem::path(filePath_).parent_path());
    }

    [[nodiscard]] std::string getRepositoryInfo() const noexcept override
    {
        return "FileActivityRepository: " + filePath_;
    }
};

} // namespace infrastructure::repositories

// Factory implementation
namespace domain::repositories {

[[nodiscard]] std::unique_ptr<IActivityRepository> createFileActivityRepository(
    const std::string& filePath)
{
    return std::make_unique<infrastructure::repositories::FileActivityRepository>(filePath);
}

} // namespace domain::repositories
