#pragma once

#include "../../domain/repositories/IStudentRepository.h"
#include "../../infrastructure/utils/Results.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace infrastructure::repositories {

// File-based Student Repository implementation
class FileStudentRepository : public domain::repositories::IStudentRepository {
private:
    std::string filePath_;

public:
    explicit FileStudentRepository(std::string filePath)
        : filePath_(std::move(filePath))
    {
    }

    // Load students từ file
    [[nodiscard]] infrastructure::utils::Result<std::vector<domain::entities::Student>>
    loadStudents() const override
    {

        if (!std::filesystem::exists(filePath_)) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::NotFound));
        }

        std::ifstream file(filePath_);
        if (!file.is_open()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::PermissionDenied));
        }

        std::vector<domain::entities::Student> students;
        std::string line;

        while (std::getline(file, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty())
                continue;

            auto studentResult = domain::entities::Student::create(line);
            if (!studentResult) {
                return infrastructure::utils::unexpected(utils::makeValidationError(utils::ValidationError::InvalidData));
            }

            students.push_back(std::move(*studentResult));
        }

        if (file.bad()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::ReadError));
        }

        return students;
    }

    // Save students to file
    [[nodiscard]] infrastructure::utils::Result<void>
    saveStudents(const std::vector<domain::entities::Student>& students) const override
    {

        std::ofstream file(filePath_);
        if (!file.is_open()) {
            return infrastructure::utils::unexpected(utils::makeFileError(utils::FileError::PermissionDenied));
        }

        for (const auto& student : students) {
            file << student.getId() << '\n';
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
        return "FileStudentRepository: " + filePath_;
    }
};

} // namespace infrastructure::repositories

// Factory implementation
namespace domain::repositories {

[[nodiscard]] std::unique_ptr<IStudentRepository> createFileStudentRepository(
    const std::string& filePath)
{
    return std::make_unique<infrastructure::repositories::FileStudentRepository>(filePath);
}

} // namespace domain::repositories
