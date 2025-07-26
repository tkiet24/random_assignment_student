#pragma once

#include <optional>
#include <regex>
#include <stdexcept>
#include <string>

namespace domain::entities {

// Error types sử dụng enum class
enum class StudentIdError {
    InvalidFormat,
    Empty
};

// Forward declarations
template <typename E>
class Unexpected;

// Result type đơn giản thay cho std::expected (compatibility)
template <typename T, typename E>
class Result {
private:
    bool hasValue_;
    union {
        T value_;
        E error_;
    };

public:
    Result(T value)
        : hasValue_(true)
        , value_(std::move(value))
    {
    }
    Result(E error)
        : hasValue_(false)
        , error_(std::move(error))
    {
    }
    Result(Unexpected<E> unexp)
        : hasValue_(false)
        , error_(std::move(unexp.error_))
    {
    }

    // Copy constructor
    Result(const Result& other)
        : hasValue_(other.hasValue_)
    {
        if (hasValue_) {
            new (&value_) T(other.value_);
        } else {
            new (&error_) E(other.error_);
        }
    }

    // Move constructor
    Result(Result&& other) noexcept
        : hasValue_(other.hasValue_)
    {
        if (hasValue_) {
            new (&value_) T(std::move(other.value_));
        } else {
            new (&error_) E(std::move(other.error_));
        }
    }

    ~Result()
    {
        if (hasValue_) {
            value_.~T();
        } else {
            error_.~E();
        }
    }

    bool has_value() const noexcept { return hasValue_; }
    operator bool() const noexcept { return hasValue_; }

    const T& value() const& { return value_; }
    T&& value() && { return std::move(value_); }
    const T& operator*() const& { return value_; }
    T&& operator*() && { return std::move(value_); }

    const E& error() const& { return error_; }
    E&& error() && { return std::move(error_); }
};

template <typename E>
class Unexpected {
public:
    E error_;
    explicit Unexpected(E error)
        : error_(std::move(error))
    {
    }
};

template <typename E>
Unexpected<E> unexpected(E error)
{
    return Unexpected<E>(std::move(error));
}

// Student entity với validation
class Student {
private:
    std::string id_;

    // Validate student ID format
    [[nodiscard]] static bool isValidId(const std::string& id) noexcept
    {
        // Regex pattern cho student ID (8 digits)
        static const std::regex pattern(R"(\d{8})");
        return std::regex_match(id, pattern);
    }

public:
    // Constructor với Result type để handle errors
    [[nodiscard]] static Result<Student, StudentIdError> create(std::string id)
    {
        if (id.empty()) {
            return unexpected(StudentIdError::Empty);
        }

        if (!isValidId(id)) {
            return unexpected(StudentIdError::InvalidFormat);
        }

        return Student { std::move(id) };
    }

    // Getter với const correctness
    [[nodiscard]] const std::string& getId() const noexcept { return id_; }

    // Comparison operators
    bool operator==(const Student& other) const = default;
    bool operator!=(const Student& other) const = default;

private:
    // Private constructor để force validation qua factory method
    explicit Student(std::string id)
        : id_(std::move(id))
    {
    }
};

} // namespace domain::entities
