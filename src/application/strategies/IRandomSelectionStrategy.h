#pragma once

#include "../../domain/entities/Activity.h"
#include <algorithm>
#include <memory>
#include <optional>
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
        domain::entities::ActivityCategory category) const = 0;

    // Get strategy name
    [[nodiscard]] virtual std::string getStrategyName() const noexcept = 0;
};

// Concrete Strategy 1: Standard Random Selection
class StandardRandomStrategy : public IRandomSelectionStrategy {
private:
    mutable std::random_device rd_;
    mutable std::mt19937 gen_;

public:
    StandardRandomStrategy();

    [[nodiscard]] std::optional<domain::entities::Activity>
    selectRandomActivity(
        const std::vector<domain::entities::Activity>& activities,
        domain::entities::ActivityCategory category) const override;

    [[nodiscard]] std::string getStrategyName() const noexcept override;
};

// Concrete Strategy 2: Weighted Random Selection
class WeightedRandomStrategy : public IRandomSelectionStrategy {
private:
    mutable std::random_device rd_;
    mutable std::mt19937 gen_;

public:
    WeightedRandomStrategy();

    [[nodiscard]] std::optional<domain::entities::Activity>
    selectRandomActivity(
        const std::vector<domain::entities::Activity>& activities,
        domain::entities::ActivityCategory category) const override;

    [[nodiscard]] std::string getStrategyName() const noexcept override;
};

// Factory functions để create strategies
[[nodiscard]] std::unique_ptr<IRandomSelectionStrategy> createStandardRandomStrategy();
[[nodiscard]] std::unique_ptr<IRandomSelectionStrategy> createWeightedRandomStrategy();

} // namespace application::strategies
