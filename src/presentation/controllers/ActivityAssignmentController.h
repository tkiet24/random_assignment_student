#pragma once

#include "../../application/services/ActivityAssignmentService.h"
#include <expected>
#include <string>
#include <memory>

namespace presentation::controllers {

// Controller class theo Clean Architecture
class ActivityAssignmentController {
private:
    std::unique_ptr<application::services::ActivityAssignmentService> service_;

public:
    explicit ActivityAssignmentController(
        std::unique_ptr<application::services::ActivityAssignmentService> service);

    // Main execution method
    [[nodiscard]] bool execute() const noexcept;

    // Method để display service info
    void displayServiceInfo() const noexcept;

private:
    // Display results
    void displayResults(
        const std::vector<application::services::ActivityAssignmentService::AssignmentResult>& results) const noexcept;

    // Display error với std::string
    void displayError(const std::string& error) const noexcept;
};

} // namespace presentation::controllers
