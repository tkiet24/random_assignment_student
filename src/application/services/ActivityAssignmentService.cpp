#include "ActivityAssignmentService.h"
#include <algorithm>
#include <ranges>

namespace application::services {

// Static member definition
constexpr std::array<domain::entities::ActivityCategory, 3> 
ActivityAssignmentService::REQUIRED_CATEGORIES;

// Constructor implementation
ActivityAssignmentService::ActivityAssignmentService(
    std::unique_ptr<domain::repositories::IStudentRepository> studentRepo,
    std::unique_ptr<domain::repositories::IActivityRepository> activityRepo,
    std::unique_ptr<strategies::IRandomSelectionStrategy> randomStrategy)
    : studentRepo_(std::move(studentRepo))
    , activityRepo_(std::move(activityRepo))
    , randomStrategy_(std::move(randomStrategy))
{
}

// AssignmentResult constructor implementation
ActivityAssignmentService::AssignmentResult::AssignmentResult(domain::entities::Student s)
    : student(std::move(s))
{
}

// Main business logic method implementation
std::optional<std::vector<ActivityAssignmentService::AssignmentResult>>
ActivityAssignmentService::assignActivitiesToStudents() const
{
    // Load data
    auto studentsOpt = studentRepo_->loadStudents();
    if (!studentsOpt) {
        return std::nullopt;
    }

    auto activitiesOpt = activityRepo_->loadActivities();
    if (!activitiesOpt) {
        return std::nullopt;
    }

    const auto& students = *studentsOpt;
    const auto& activities = *activitiesOpt;

    // Validate có đủ activities cho mỗi category
    if (!validateActivitiesAvailable(activities)) {
        return std::nullopt;
    }

    std::vector<AssignmentResult> results;
    results.reserve(students.size());

    // Process each student
    for (const auto& student : students) {
        auto assignmentOpt = assignActivitiesToStudent(student, activities);
        if (!assignmentOpt) {
            return std::nullopt;
        }
        results.push_back(std::move(*assignmentOpt));
    }

    return results;
}

// Method để change strategy at runtime (Strategy Pattern)
void ActivityAssignmentService::setRandomStrategy(
    std::unique_ptr<strategies::IRandomSelectionStrategy> strategy)
{
    randomStrategy_ = std::move(strategy);
}

// Get current strategy info
std::string ActivityAssignmentService::getCurrentStrategyInfo() const noexcept
{
    return randomStrategy_ ? randomStrategy_->getStrategyName() : "No strategy set";
}

// Helper method để validate activities sử dụng C++20 ranges
bool ActivityAssignmentService::validateActivitiesAvailable(
    const std::vector<domain::entities::Activity>& activities) const noexcept
{
    // Sử dụng ranges::all_of (C++20) để check tất cả categories có activities
    return std::ranges::all_of(REQUIRED_CATEGORIES, [&activities](auto category) {
        return std::ranges::any_of(activities, [category](const auto& activity) {
            return activity.getCategory() == category;
        });
    });
}

// Helper method to assign activities to a single student
std::optional<ActivityAssignmentService::AssignmentResult>
ActivityAssignmentService::assignActivitiesToStudent(
    const domain::entities::Student& student,
    const std::vector<domain::entities::Activity>& activities) const
{
    AssignmentResult result{student};

    // Assign one activity per category
    for (size_t i = 0; i < REQUIRED_CATEGORIES.size(); ++i) {
        auto activityOpt = randomStrategy_->selectRandomActivity(
            activities, REQUIRED_CATEGORIES[i]);

        if (!activityOpt) {
            return std::nullopt;
        }

        result.activities[i] = std::move(*activityOpt);
    }

    return result;
}

} // namespace application::services
