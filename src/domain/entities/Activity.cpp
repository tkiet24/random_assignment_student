#include "Activity.h"

namespace domain::entities {

// Constructor implementations
Activity::Activity() : name_(""), category_(ActivityCategory::Class) {}

Activity::Activity(std::string name, ActivityCategory category) 
    : name_(std::move(name)), category_(category) {}

// Getter implementations  
const std::string& Activity::getName() const noexcept { 
    return name_; 
}

ActivityCategory Activity::getCategory() const noexcept { 
    return category_; 
}

// Utility function implementations
std::string Activity::getFormattedActivity() const {
    return name_ + " (" + categoryToString(category_) + ")";
}

// Static utility functions
std::string Activity::categoryToString(ActivityCategory category) {
    switch (category) {
        case ActivityCategory::Class: return "Class";
        case ActivityCategory::Union: return "Union";  
        case ActivityCategory::School: return "School";
        default: return "Unknown";
    }
}

std::optional<ActivityCategory> Activity::stringToCategory(const std::string& str) {
    if (str == "Class") return ActivityCategory::Class;
    if (str == "Union") return ActivityCategory::Union;
    if (str == "School") return ActivityCategory::School;
    return std::nullopt;
}

} // namespace domain::entities
