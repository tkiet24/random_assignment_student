#pragma once

#include "../../application/strategies/IRandomSelectionStrategy.h"
#include "../../domain/entities/Activity.h"
#include "../../domain/entities/Student.h"
#include "../../domain/repositories/IActivityRepository.h"
#include "../../domain/repositories/IStudentRepository.h"
#include <array>
#include <expected>
#include <memory>
#include <vector>
#include <string>

namespace application::services {

// Service class theo Clean Architecture
class ActivityAssignmentService {
private:
    std::unique_ptr<domain::repositories::IStudentRepository> studentRepo_;
    std::unique_ptr<domain::repositories::IActivityRepository> activityRepo_;
    std::unique_ptr<strategies::IRandomSelectionStrategy> randomStrategy_;

    // std::array (C++11) để store required categories
    static constexpr std::array<domain::entities::ActivityCategory, 3> REQUIRED_CATEGORIES = {
        domain::entities::ActivityCategory::Class,
        domain::entities::ActivityCategory::Union,
        domain::entities::ActivityCategory::School
    };

public:
    // Constructor với dependency injection
    ActivityAssignmentService(
        std::unique_ptr<domain::repositories::IStudentRepository> studentRepo,
        std::unique_ptr<domain::repositories::IActivityRepository> activityRepo,
        std::unique_ptr<strategies::IRandomSelectionStrategy> randomStrategy);

    // Structured binding return type (C++17)
    struct AssignmentResult {
        domain::entities::Student student;
        std::array<domain::entities::Activity, 3> activities;

        // Constructor với Student
        AssignmentResult(domain::entities::Student s);
    };

    // Main business logic method
    [[nodiscard]] std::optional<std::vector<AssignmentResult>>
    assignActivitiesToStudents() const;

    // Method để change strategy at runtime (Strategy Pattern)
    void setRandomStrategy(std::unique_ptr<strategies::IRandomSelectionStrategy> strategy);

    // Get current strategy info
    [[nodiscard]] std::string getCurrentStrategyInfo() const noexcept;

private:
    // Helper method để validate activities
    [[nodiscard]] bool validateActivitiesAvailable(
        const std::vector<domain::entities::Activity>& activities) const noexcept;

    // Helper method to assign activities to a single student
    [[nodiscard]] std::optional<AssignmentResult>
    assignActivitiesToStudent(
        const domain::entities::Student& student,
        const std::vector<domain::entities::Activity>& activities) const;
};

} // namespace application::services
