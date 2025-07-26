#pragma once

#include "../../domain/entities/Activity.h"
#include <expected>
#include <memory>
#include <vector>
#include <string>

namespace domain::repositories {

// Repository interface for Activity entity
class IActivityRepository {
public:
    virtual ~IActivityRepository() = default;

    // Load activities với std::expected
    [[nodiscard]] virtual std::expected<std::vector<entities::Activity>, std::string>
    loadActivities() const = 0;

    // Save activities với std::expected
    [[nodiscard]] virtual std::expected<void, std::string>
    saveActivities(const std::vector<entities::Activity>& activities) const = 0;

    // Check if repository is available
    [[nodiscard]] virtual bool isAvailable() const noexcept = 0;

    // Get repository info
    [[nodiscard]] virtual std::string getRepositoryInfo() const noexcept = 0;
};

// Factory function để tạo repository instance
[[nodiscard]] std::unique_ptr<IActivityRepository> createFileActivityRepository(
    const std::string& filePath);

} // namespace domain::repositories
