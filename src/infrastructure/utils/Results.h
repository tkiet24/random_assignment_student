#pragma once

#include <string>
#include <system_error>
#include <variant>

namespace infrastructure::utils {

// Forward declaration
template <typename E>
class Unexpected;

// Error types sử dụng enum class
enum class FileError {
    NotFound,
    PermissionDenied,
    ReadError,
    InvalidFormat
};

enum class ValidationError {
    InvalidData,
    MissingField,
    FormatError
};

// std::variant (C++17) để combine different error types
using ApplicationError = std::variant<
    FileError,
    ValidationError,
    std::error_code>;

// Result type template (compatible version)
template <typename T>
class Result {
private:
    bool hasValue_;
    union {
        T value_;
        ApplicationError error_;
    };

public:
    Result(T value)
        : hasValue_(true)
        , value_(std::move(value))
    {
    }
    Result(ApplicationError error)
        : hasValue_(false)
        , error_(std::move(error))
    {
    }

    template <typename E>
    Result(Unexpected<E> unexp)
        : hasValue_(false)
        , error_(std::move(unexp.error_))
    {
    }

    ~Result()
    {
        if (hasValue_) {
            value_.~T();
        } else {
            error_.~ApplicationError();
        }
    }

    // Copy constructor
    Result(const Result& other)
        : hasValue_(other.hasValue_)
    {
        if (hasValue_) {
            new (&value_) T(other.value_);
        } else {
            new (&error_) ApplicationError(other.error_);
        }
    }

    // Move constructor
    Result(Result&& other) noexcept
        : hasValue_(other.hasValue_)
    {
        if (hasValue_) {
            new (&value_) T(std::move(other.value_));
        } else {
            new (&error_) ApplicationError(std::move(other.error_));
        }
    }

    bool has_value() const noexcept { return hasValue_; }
    operator bool() const noexcept { return hasValue_; }

    const T& value() const& { return value_; }
    T&& value() && { return std::move(value_); }
    const T& operator*() const& { return value_; }
    T&& operator*() && { return std::move(value_); }

    const ApplicationError& error() const& { return error_; }
    ApplicationError&& error() && { return std::move(error_); }
};

// Specialization for void
template <>
class Result<void> {
private:
    bool hasValue_;
    ApplicationError error_;

public:
    Result()
        : hasValue_(true)
    {
    }
    Result(ApplicationError error)
        : hasValue_(false)
        , error_(std::move(error))
    {
    }

    template <typename E>
    Result(Unexpected<E> unexp)
        : hasValue_(false)
        , error_(std::move(unexp.error_))
    {
    }

    bool has_value() const noexcept { return hasValue_; }
    operator bool() const noexcept { return hasValue_; }

    const ApplicationError& error() const& { return error_; }
    ApplicationError&& error() && { return std::move(error_); }
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

// Utility functions để create errors
[[nodiscard]] inline ApplicationError makeFileError(FileError error)
{
    return error;
}

[[nodiscard]] inline ApplicationError makeValidationError(ValidationError error)
{
    return error;
}

[[nodiscard]] inline ApplicationError makeSystemError(std::error_code ec)
{
    return ec;
}

// Error message formatter
[[nodiscard]] std::string formatError(const ApplicationError& error)
{
    return std::visit([](const auto& err) -> std::string {
        using ErrorType = std::decay_t<decltype(err)>;

        if constexpr (std::is_same_v<ErrorType, FileError>) {
            switch (err) {
            case FileError::NotFound:
                return "File not found";
            case FileError::PermissionDenied:
                return "Permission denied";
            case FileError::ReadError:
                return "Read error";
            case FileError::InvalidFormat:
                return "Invalid file format";
            default:
                return "Unknown file error";
            }
        } else if constexpr (std::is_same_v<ErrorType, ValidationError>) {
            switch (err) {
            case ValidationError::InvalidData:
                return "Invalid data";
            case ValidationError::MissingField:
                return "Missing required field";
            case ValidationError::FormatError:
                return "Format error";
            default:
                return "Unknown validation error";
            }
        } else if constexpr (std::is_same_v<ErrorType, std::error_code>) {
            return err.message();
        } else {
            return "Unknown error";
        }
    },
        error);
}

} // namespace infrastructure::utils
