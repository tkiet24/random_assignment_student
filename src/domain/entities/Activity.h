#pragma once

#include <optional>
#include <string>

namespace domain::entities {

// Enum class cho category với C++11 scoped enums
enum class ActivityCategory {
    Class,
    Union,
    School
};

// Activity entity với các tính năng C++ hiện đại
class Activity {
private:
    std::string name_;
    ActivityCategory category_;

public:
    // Constructors
    Activity();
    Activity(std::string name, ActivityCategory category);

    // Getters với const correctness
    [[nodiscard]] const std::string& getName() const noexcept;
    [[nodiscard]] ActivityCategory getCategory() const noexcept;

    // Utility function để format activity
    [[nodiscard]] std::string getFormattedActivity() const;

    // Static utility function để convert enum sang string
    [[nodiscard]] static std::string categoryToString(ActivityCategory category);

    // Static function để parse string thành ActivityCategory với std::optional
    [[nodiscard]] static std::optional<ActivityCategory> stringToCategory(const std::string& str);

    // Equality operators
    bool operator==(const Activity& other) const = default;
    bool operator!=(const Activity& other) const = default;
};

} // namespace domain::entities
