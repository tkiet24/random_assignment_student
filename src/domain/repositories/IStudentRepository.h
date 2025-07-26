#pragma once

#include "../../domain/entities/Student.h"
#include "../../infrastructure/utils/Results.h"
#include <memory>
#include <vector>

namespace domain::repositories {

// Repository interface sử dụng Repository Pattern
class IStudentRepository {
public:
    virtual ~IStudentRepository() = default;

    // Load students với Result type
    [[nodiscard]] virtual infrastructure::utils::Result<std::vector<entities::Student>>
    loadStudents() const = 0;

    // Save students với Result type
    [[nodiscard]] virtual infrastructure::utils::Result<void>
    saveStudents(const std::vector<entities::Student>& students) const = 0;

    // Check if repository is available
    [[nodiscard]] virtual bool isAvailable() const noexcept = 0;

    // Get repository info
    [[nodiscard]] virtual std::string getRepositoryInfo() const noexcept = 0;
};

// Factory function để tạo repository instance
[[nodiscard]] std::unique_ptr<IStudentRepository> createFileStudentRepository(
    const std::string& filePath);

} // namespace domain::repositories
