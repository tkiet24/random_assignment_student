#pragma once

#include "../../application/services/ActivityAssignmentService.h"
#include "../../infrastructure/utils/Results.h"
#include <iostream>
#include <string_view>

namespace presentation::controllers {

// Controller class theo Clean Architecture
class ActivityAssignmentController {
private:
    std::unique_ptr<application::services::ActivityAssignmentService> service_;

public:
    explicit ActivityAssignmentController(
        std::unique_ptr<application::services::ActivityAssignmentService> service)
        : service_(std::move(service))
    {
    }

    // Main execution method
    [[nodiscard]] bool execute() const noexcept
    {
        try {
            auto result = service_->assignActivitiesToStudents();

            if (!result) {
                displayError(result.error());
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

    // Method để display service info
    void displayServiceInfo() const noexcept
    {
        std::cout << "Current strategy: " << service_->getCurrentStrategyInfo() << "\n";
    }

private:
    // Display results
    void displayResults(
        const std::vector<application::services::ActivityAssignmentService::AssignmentResult>& results) const noexcept
    {

        for (const auto& result : results) { // Structured binding alternative
            const auto& student = result.student;
            const auto& activities = result.activities;

            std::cout << student.getId() << ": "
                      << activities[0].getFormattedActivity() << ", "
                      << activities[1].getFormattedActivity() << ", "
                      << activities[2].getFormattedActivity() << "\n";
        }
    }

    // Display error với visitor pattern cho std::variant
    void displayError(const infrastructure::utils::ApplicationError& error) const noexcept
    {
        std::cerr << "Error: " << infrastructure::utils::formatError(error) << "\n";
    }
};

} // namespace presentation::controllers
