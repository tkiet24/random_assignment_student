#pragma once

#include "../../domain/repositories/IActivityRepository.h"
#include <expected>
#include <string>

namespace infrastructure::repositories {

// File-based Activity Repository implementation
class FileActivityRepository : public domain::repositories::IActivityRepository {
private:
    std::string filePath_;

public:
    explicit FileActivityRepository(std::string filePath);

    // Load activities từ file
    [[nodiscard]] std::expected<std::vector<domain::entities::Activity>, std::string>
    loadActivities() const override;

    // Save activities to file
    [[nodiscard]] std::expected<void, std::string>
    saveActivities(const std::vector<domain::entities::Activity>& activities) const override;

    [[nodiscard]] bool isAvailable() const noexcept override;

    [[nodiscard]] std::string getRepositoryInfo() const noexcept override;
};

} // namespace infrastructure::repositories

// Factory function declaration
namespace domain::repositories {

[[nodiscard]] std::unique_ptr<IActivityRepository> createFileActivityRepository(
    const std::string& filePath);

} // namespace domain::repositories
