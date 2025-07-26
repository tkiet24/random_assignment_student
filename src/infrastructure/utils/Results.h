#pragma once

#include <expected>
#include <string>
#include <system_error>
#include <variant>

namespace infrastructure::utils {

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

// Sử dụng std::expected từ C++23 thay vì custom Result
template <typename T>
using Result = std::expected<T, ApplicationError>;

// Helper functions để tạo errors
[[nodiscard]] constexpr ApplicationError makeFileError(FileError error) noexcept
{
    return error;
}

[[nodiscard]] constexpr ApplicationError makeValidationError(ValidationError error) noexcept
{
    return error;
}

// Error string conversion functions
[[nodiscard]] std::string toString(FileError error)
{
    switch (error) {
    case FileError::NotFound:
        return "File not found";
    case FileError::PermissionDenied:
        return "Permission denied";
    case FileError::ReadError:
        return "Read error";
    case FileError::InvalidFormat:
        return "Invalid format";
    default:
        return "Unknown file error";
    }
}

[[nodiscard]] std::string toString(ValidationError error)
{
    switch (error) {
    case ValidationError::InvalidData:
        return "Invalid data";
    case ValidationError::MissingField:
        return "Missing field";
    case ValidationError::FormatError:
        return "Format error";
    default:
        return "Unknown validation error";
    }
}

[[nodiscard]] std::string toString(const ApplicationError& error)
{
    return std::visit([](const auto& e) -> std::string {
        if constexpr (std::is_same_v<std::decay_t<decltype(e)>, FileError>) {
            return toString(e);
        } else if constexpr (std::is_same_v<std::decay_t<decltype(e)>, ValidationError>) {
            return toString(e);
        } else {
            return e.message();
        }
    },
        error);
}

// Alias for backward compatibility
[[nodiscard]] inline std::string formatError(const ApplicationError& error)
{
    return toString(error);
}

} // namespace infrastructure::utils
