#include "src/application/services/ActivityAssignmentService.h"
#include "src/application/strategies/IRandomSelectionStrategy.h"
#include "src/domain/repositories/IActivityRepository.h"
#include "src/domain/repositories/IStudentRepository.h"
#include "src/infrastructure/repositories/FileActivityRepository.h"
#include "src/infrastructure/repositories/FileStudentRepository.h"
#include "src/presentation/controllers/ActivityAssignmentController.h"
#include <iostream>
#include <string_view>

// Nested namespace definitions (C++17)
namespace app::config {
constexpr std::string_view STUDENTS_FILE = "students.txt";
constexpr std::string_view ACTIVITIES_FILE = "activities.txt";
}

namespace app::factory {

// Factory class để create application dependencies
class ApplicationFactory {
public:
    // if constexpr template để choose strategy based on template parameter
    template <bool UseWeightedStrategy = false>
    [[nodiscard]] static std::unique_ptr<presentation::controllers::ActivityAssignmentController>
    createController()
    {

        // Create repositories
        auto studentRepo = domain::repositories::createFileStudentRepository(
            std::string { app::config::STUDENTS_FILE });
        auto activityRepo = domain::repositories::createFileActivityRepository(
            std::string { app::config::ACTIVITIES_FILE });

        // Create strategy based on template parameter (if constexpr - C++17)
        std::unique_ptr<application::strategies::IRandomSelectionStrategy> strategy;
        if constexpr (UseWeightedStrategy) {
            strategy = application::strategies::createWeightedRandomStrategy();
        } else {
            strategy = application::strategies::createStandardRandomStrategy();
        }

        // Create service with dependency injection
        auto service = std::make_unique<application::services::ActivityAssignmentService>(
            std::move(studentRepo), std::move(activityRepo), std::move(strategy));

        // Create controller
        return std::make_unique<presentation::controllers::ActivityAssignmentController>(
            std::move(service));
    }
};

} // namespace app::factory

int main()
{
    try {
        std::cout << "Student Activity Assignment System\n";
        std::cout << "==================================\n\n";

        // Create controller với standard strategy
        auto controller = app::factory::ApplicationFactory::createController<false>();

        // Display strategy info
        controller->displayServiceInfo();
        std::cout << "\n";

        // Execute assignment
        bool success = controller->execute();

        if (!success) {
            std::cerr << "\nApplication failed to complete successfully.\n";
            return 1;
        }

        std::cout << "\nAssignment completed successfully!\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred\n";
        return 1;
    }
}
