#pragma once

#include "../../domain/entities/Student.h"
#include <expected>
#include <memory>
#include <vector>
#include <string>

namespace domain::repositories {

#pragma once

#include "../../domain/entities/Student.h"
#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace domain::repositories {

// Repository interface for Student entity
class IStudentRepository {
public:
    virtual ~IStudentRepository() = default;

    // Load students với std::optional for error handling
    [[nodiscard]] virtual std::optional<std::vector<entities::Student>>
    loadStudents() const = 0;

    // Save students (returns false on error)
    [[nodiscard]] virtual bool
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
class IStudentRepository {
public:
    virtual ~IStudentRepository() = default;

    // Load students với std::expected
    [[nodiscard]] virtual std::expected<std::vector<entities::Student>, std::string>
    loadStudents() const = 0;

    // Save students với std::expected
    [[nodiscard]] virtual std::expected<void, std::string>
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
