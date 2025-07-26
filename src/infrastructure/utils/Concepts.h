#pragma once

#include <concepts>
#include <ranges>
#include <type_traits>
#include <vector>

namespace infrastructure::utils {

// Concepts (C++20) để định nghĩa constraints
template <typename T>
concept Randomizable = requires(T t) {
    { t.begin() } -> std::forward_iterator;
    { t.end() } -> std::forward_iterator;
    { t.size() } -> std::convertible_to<std::size_t>;
    requires !std::is_empty_v<T>;
};

template <typename T>
concept HasCategory = requires(T t) {
    { t.getCategory() };
};

// Concept cho container của activities
template <typename Container>
concept ActivityContainer = Randomizable<Container> && HasCategory<typename Container::value_type>;

// if constexpr template để check container properties (C++17)
template <typename Container>
constexpr bool is_contiguous_container_v = false;

template <typename T, typename A>
constexpr bool is_contiguous_container_v<std::vector<T, A>> = true;

// Template với if constexpr (C++17) để optimize operations
template <typename Container>
constexpr auto get_container_info()
{
    if constexpr (is_contiguous_container_v<Container>) {
        return "Contiguous container - optimized access";
    } else {
        return "Non-contiguous container - standard access";
    }
}

} // namespace infrastructure::utils
