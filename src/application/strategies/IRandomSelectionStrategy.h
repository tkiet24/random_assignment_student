#pragma once

#include "../../domain/entities/Activity.h"
#include "../../infrastructure/utils/Concepts.h"
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

namespace application::strategies {

// Strategy Pattern - Abstract strategy interface
class IRandomSelectionStrategy {
public:
    virtual ~IRandomSelectionStrategy() = default;

    // Strategy method để select random activity
    [[nodiscard]] virtual std::optional<domain::entities::Activity>
    selectRandomActivity(
        const std::vector<domain::entities::Activity>& activities,
        domain::entities::ActivityCategory category) const
        = 0;

    // Get strategy name
    [[nodiscard]] virtual std::string getStrategyName() const noexcept = 0;
};

// Concrete Strategy 1: Standard Random Selection
class StandardRandomStrategy : public IRandomSelectionStrategy {
private:
    mutable std::random_device rd_;
    mutable std::mt19937 gen_;

public:
    StandardRandomStrategy()
        : gen_(rd_())
    {
    }

    [[nodiscard]] std::optional<domain::entities::Activity>
    selectRandomActivity(
        const std::vector<domain::entities::Activity>& activities,
        domain::entities::ActivityCategory category) const override
    {

        // Filter activities by category
        std::vector<domain::entities::Activity> categoryActivities;
        for (const auto& activity : activities) {
            if (activity.getCategory() == category) {
                categoryActivities.push_back(activity);
            }
        }

        if (categoryActivities.empty()) {
            return std::nullopt;
        }

        std::uniform_int_distribution<size_t> dist(0, categoryActivities.size() - 1);
        return categoryActivities[dist(gen_)];
    }

    [[nodiscard]] std::string getStrategyName() const noexcept override
    {
        return "StandardRandomStrategy";
    }
};

// Concrete Strategy 2: Weighted Random Selection (advanced)
class WeightedRandomStrategy : public IRandomSelectionStrategy {
private:
    mutable std::random_device rd_;
    mutable std::mt19937 gen_;

public:
    WeightedRandomStrategy()
        : gen_(rd_())
    {
    }

    [[nodiscard]] std::optional<domain::entities::Activity>
    selectRandomActivity(
        const std::vector<domain::entities::Activity>& activities,
        domain::entities::ActivityCategory category) const override
    {

        // Filter activities by category
        std::vector<domain::entities::Activity> categoryActivities;
        for (const auto& activity : activities) {
            if (activity.getCategory() == category) {
                categoryActivities.push_back(activity);
            }
        }

        if (categoryActivities.empty()) {
            return std::nullopt;
        }

        // Weighted selection based on name length (just for demo)
        std::vector<double> weights;
        weights.reserve(categoryActivities.size());

        for (const auto& activity : categoryActivities) {
            // Weight based on inverse name length để favor shorter names
            weights.push_back(1.0 / std::max(1.0, static_cast<double>(activity.getName().length())));
        }

        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        return categoryActivities[dist(gen_)];
    }

    [[nodiscard]] std::string getStrategyName() const noexcept override
    {
        return "WeightedRandomStrategy";
    }
};

// Factory functions để create strategies
[[nodiscard]] inline std::unique_ptr<IRandomSelectionStrategy> createStandardRandomStrategy()
{
    return std::make_unique<StandardRandomStrategy>();
}

[[nodiscard]] inline std::unique_ptr<IRandomSelectionStrategy> createWeightedRandomStrategy()
{
    return std::make_unique<WeightedRandomStrategy>();
}

} // namespace application::strategies
