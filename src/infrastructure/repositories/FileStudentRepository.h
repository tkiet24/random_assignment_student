#pragma once

#include "../../domain/repositories/IStudentRepository.h"
#include <optional>
#include <string>

namespace infrastructure::repositories {

// File-based Student Repository implementation
class FileStudentRepository : public domain::repositories::IStudentRepository {
private:
    std::string filePath_;

public:
    explicit FileStudentRepository(std::string filePath);

    // Load students từ file
    [[nodiscard]] std::optional<std::vector<domain::entities::Student>>
    loadStudents() const override;

    // Save students to file
    [[nodiscard]] bool
    saveStudents(const std::vector<domain::entities::Student>& students) const override;

    [[nodiscard]] bool isAvailable() const noexcept override;

    [[nodiscard]] std::string getRepositoryInfo() const noexcept override;
};

} // namespace infrastructure::repositories

// Factory function declaration
namespace domain::repositories {

[[nodiscard]] std::unique_ptr<IStudentRepository> createFileStudentRepository(
    const std::string& filePath);

} // namespace domain::repositories
