#include "IRandomSelectionStrategy.h"
#include <ranges>
#include <algorithm>

namespace application::strategies {

// StandardRandomStrategy implementation
StandardRandomStrategy::StandardRandomStrategy() : gen_(rd_()) {}

std::optional<domain::entities::Activity>
StandardRandomStrategy::selectRandomActivity(
    const std::vector<domain::entities::Activity>& activities,
    domain::entities::ActivityCategory category) const {
    
    // Sử dụng C++20 ranges để filter activities by category
    auto categoryActivities = activities 
        | std::views::filter([category](const auto& activity) {
            return activity.getCategory() == category;
        });
    
    // Convert view to vector for random selection
    std::vector<domain::entities::Activity> filteredActivities(
        categoryActivities.begin(), categoryActivities.end());

    if (filteredActivities.empty()) {
        return std::nullopt;
    }

    std::uniform_int_distribution<size_t> dist(0, filteredActivities.size() - 1);
    return filteredActivities[dist(gen_)];
}

std::string StandardRandomStrategy::getStrategyName() const noexcept {
    return "StandardRandomStrategy";
}

// WeightedRandomStrategy implementation
WeightedRandomStrategy::WeightedRandomStrategy() : gen_(rd_()) {}

std::optional<domain::entities::Activity>
WeightedRandomStrategy::selectRandomActivity(
    const std::vector<domain::entities::Activity>& activities,
    domain::entities::ActivityCategory category) const {
    
    // Sử dụng C++20 ranges để filter activities by category
    auto categoryActivities = activities 
        | std::views::filter([category](const auto& activity) {
            return activity.getCategory() == category;
        });
    
    // Convert view to vector for weighted selection
    std::vector<domain::entities::Activity> filteredActivities(
        categoryActivities.begin(), categoryActivities.end());

    if (filteredActivities.empty()) {
        return std::nullopt;
    }

    // Weighted selection based on name length (just for demo)
    std::vector<double> weights;
    weights.reserve(filteredActivities.size());

    for (const auto& activity : filteredActivities) {
        // Weight based on inverse name length để favor shorter names
        weights.push_back(1.0 / std::max(1.0, static_cast<double>(activity.getName().length())));
    }

    std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
    return filteredActivities[dist(gen_)];
}

std::string WeightedRandomStrategy::getStrategyName() const noexcept {
    return "WeightedRandomStrategy";
}

// Factory functions implementation
std::unique_ptr<IRandomSelectionStrategy> createStandardRandomStrategy() {
    return std::make_unique<StandardRandomStrategy>();
}

std::unique_ptr<IRandomSelectionStrategy> createWeightedRandomStrategy() {
    return std::make_unique<WeightedRandomStrategy>();
}

} // namespace application::strategies
