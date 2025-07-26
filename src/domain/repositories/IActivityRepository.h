#pragma once

#include "../../domain/entities/Activity.h"
#include "../../infrastructure/utils/Results.h"
#include <memory>
#include <vector>

namespace domain::repositories {

// Repository interface cho Activity
class IActivityRepository {
public:
    virtual ~IActivityRepository() = default;

    // Load activities với Result type
    [[nodiscard]] virtual infrastructure::utils::Result<std::vector<entities::Activity>>
    loadActivities() const = 0;

    // Save activities với Result type
    [[nodiscard]] virtual infrastructure::utils::Result<void>
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
