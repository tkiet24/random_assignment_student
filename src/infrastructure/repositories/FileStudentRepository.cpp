#include "FileStudentRepository.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cctype>

namespace infrastructure::repositories {

FileStudentRepository::FileStudentRepository(std::string filePath)
    : filePath_(std::move(filePath))
{
}

std::optional<std::vector<domain::entities::Student>>
FileStudentRepository::loadStudents() const
{
    if (!std::filesystem::exists(filePath_)) {
        return std::nullopt;
    }

    std::ifstream file(filePath_);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::vector<domain::entities::Student> students;
    std::string line;

    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty())
            continue;

        // Simple validation: just check if line is not empty
        if (line.length() == 8 && std::all_of(line.begin(), line.end(), ::isdigit)) {
            students.emplace_back(line);
        }
    }

    if (file.bad()) {
        return std::nullopt;
    }

    return students;
}

bool
FileStudentRepository::saveStudents(const std::vector<domain::entities::Student>& students) const
{
    std::ofstream file(filePath_);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& student : students) {
        file << student.getId() << '\n';
    }

    return !file.bad();
}

bool FileStudentRepository::isAvailable() const noexcept
{
    return std::filesystem::exists(filePath_) || std::filesystem::exists(std::filesystem::path(filePath_).parent_path());
}

std::string FileStudentRepository::getRepositoryInfo() const noexcept
{
    return "FileStudentRepository: " + filePath_;
}

} // namespace infrastructure::repositories

// Factory implementation
namespace domain::repositories {

std::unique_ptr<IStudentRepository> createFileStudentRepository(
    const std::string& filePath)
{
    return std::make_unique<infrastructure::repositories::FileStudentRepository>(filePath);
}

} // namespace domain::repositories
