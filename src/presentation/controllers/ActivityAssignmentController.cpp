#include "ActivityAssignmentController.h"
#include <iostream>

namespace presentation::controllers {

ActivityAssignmentController::ActivityAssignmentController(
    std::unique_ptr<application::services::ActivityAssignmentService> service)
    : service_(std::move(service))
{
}

bool ActivityAssignmentController::execute() const noexcept
{
    try {
        auto result = service_->assignActivitiesToStudents();

        if (!result) {
            displayError("Failed to assign activities to students");
            return false;
        }

        displayResults(*result);
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return false;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return false;
    }
}

void ActivityAssignmentController::displayServiceInfo() const noexcept
{
    std::cout << "Current strategy: " << service_->getCurrentStrategyInfo() << "\n";
}

void ActivityAssignmentController::displayResults(
    const std::vector<application::services::ActivityAssignmentService::AssignmentResult>& results) const noexcept
{
    for (const auto& [student, activities] : results) { // C++17 Structured Bindings
        std::cout << student.getId() << ": "
                  << activities[0].getFormattedActivity() << ", "
                  << activities[1].getFormattedActivity() << ", "
                  << activities[2].getFormattedActivity() << "\n";
    }
}

void ActivityAssignmentController::displayError(const std::string& error) const noexcept
{
    std::cerr << "Error: " << error << "\n";
}

} // namespace presentation::controllers
