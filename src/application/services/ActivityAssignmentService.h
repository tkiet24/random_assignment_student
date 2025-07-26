#pragma once

#include "../../application/strategies/IRandomSelectionStrategy.h"
#include "../../domain/entities/Activity.h"
#include "../../domain/entities/Student.h"
#include "../../domain/repositories/IActivityRepository.h"
#include "../../domain/repositories/IStudentRepository.h"
#include "../../infrastructure/utils/Results.h"
#include <array>
#include <memory>
#include <vector>

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
        std::unique_ptr<strategies::IRandomSelectionStrategy> randomStrategy)
        : studentRepo_(std::move(studentRepo))
        , activityRepo_(std::move(activityRepo))
        , randomStrategy_(std::move(randomStrategy))
    {
    }

    // Structured binding return type (C++17)
    struct AssignmentResult {
        domain::entities::Student student;
        std::array<domain::entities::Activity, 3> activities;

        // Constructor với Student
        AssignmentResult(domain::entities::Student s)
            : student(std::move(s))
        {
        }
    };

    // Main business logic method
    [[nodiscard]] infrastructure::utils::Result<std::vector<AssignmentResult>>
    assignActivitiesToStudents() const
    {

        // Load data
        auto studentsResult = studentRepo_->loadStudents();
        if (!studentsResult) {
            return infrastructure::utils::unexpected(studentsResult.error());
        }

        auto activitiesResult = activityRepo_->loadActivities();
        if (!activitiesResult) {
            return infrastructure::utils::unexpected(activitiesResult.error());
        }

        const auto& students = *studentsResult;
        const auto& activities = *activitiesResult;

        // Validate có đủ activities cho mỗi category
        if (!validateActivitiesAvailable(activities)) {
            return infrastructure::utils::unexpected(infrastructure::utils::makeValidationError(
                infrastructure::utils::ValidationError::InvalidData));
        }

        std::vector<AssignmentResult> results;
        results.reserve(students.size());

        // Process each student
        for (const auto& student : students) {
            auto assignmentResult = assignActivitiesToStudent(student, activities);
            if (!assignmentResult) {
                return infrastructure::utils::unexpected(assignmentResult.error());
            }
            results.push_back(std::move(*assignmentResult));
        }

        return results;
    }

    // Method để change strategy at runtime (Strategy Pattern)
    void setRandomStrategy(std::unique_ptr<strategies::IRandomSelectionStrategy> strategy)
    {
        randomStrategy_ = std::move(strategy);
    }

    // Get current strategy info
    [[nodiscard]] std::string getCurrentStrategyInfo() const noexcept
    {
        return randomStrategy_ ? randomStrategy_->getStrategyName() : "No strategy set";
    }

private:
    // Helper method để validate activities
    [[nodiscard]] bool validateActivitiesAvailable(
        const std::vector<domain::entities::Activity>& activities) const noexcept
    {

        for (auto category : REQUIRED_CATEGORIES) {
            bool found = false;
            for (const auto& activity : activities) {
                if (activity.getCategory() == category) {
                    found = true;
                    break;
                }
            }
            if (!found)
                return false;
        }
        return true;
    }

    // Helper method to assign activities to a single student
    [[nodiscard]] infrastructure::utils::Result<AssignmentResult>
    assignActivitiesToStudent(
        const domain::entities::Student& student,
        const std::vector<domain::entities::Activity>& activities) const
    {

        AssignmentResult result { student };

        // Assign one activity per category
        for (size_t i = 0; i < REQUIRED_CATEGORIES.size(); ++i) {
            auto activityOpt = randomStrategy_->selectRandomActivity(
                activities, REQUIRED_CATEGORIES[i]);

            if (!activityOpt) {
                return infrastructure::utils::unexpected(infrastructure::utils::makeValidationError(
                    infrastructure::utils::ValidationError::InvalidData));
            }

            result.activities[i] = std::move(*activityOpt);
        }

        return result;
    }
};

} // namespace application::services
