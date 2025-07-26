#pragma once

#include <optional>
#include <string>

// Compatibility với format function
#include <sstream>
namespace std {
template <typename... Args>
std::string format(const std::string& fmt, Args&&... args)
{
    // Simplified format implementation for compatibility
    std::ostringstream oss;
    oss << fmt;
    return oss.str();
}
}

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
    // Default constructor
    Activity()
        : name_("")
        , category_(ActivityCategory::Class)
    {
    }

    // Constructor với initializer list
    Activity(std::string name, ActivityCategory category)
        : name_(std::move(name))
        , category_(category)
    {
    }

    // Getters với const correctness
    [[nodiscard]] const std::string& getName() const noexcept { return name_; }
    [[nodiscard]] ActivityCategory getCategory() const noexcept { return category_; }

    // Utility function để format activity
    [[nodiscard]] std::string getFormattedActivity() const
    {
        return name_ + " (" + categoryToString(category_) + ")";
    }

    // Static utility function để convert enum sang string
    [[nodiscard]] static std::string categoryToString(ActivityCategory category)
    {
        switch (category) {
        case ActivityCategory::Class:
            return "Class";
        case ActivityCategory::Union:
            return "Union";
        case ActivityCategory::School:
            return "School";
        default:
            return "Unknown";
        }
    }

    // Static function để parse string thành ActivityCategory với std::optional
    [[nodiscard]] static std::optional<ActivityCategory> stringToCategory(const std::string& str)
    {
        if (str == "Class")
            return ActivityCategory::Class;
        if (str == "Union")
            return ActivityCategory::Union;
        if (str == "School")
            return ActivityCategory::School;
        return std::nullopt;
    }

    // Equality operators
    bool operator==(const Activity& other) const = default;
    bool operator!=(const Activity& other) const = default;
};

} // namespace domain::entities
