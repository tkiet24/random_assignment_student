#pragma once
#include <string>

namespace domain::entities {

class Student {
public:
    std::string id;

    explicit Student(std::string studentId);

    const std::string& getId() const;

    bool operator==(const Student& other) const = default;
};

} // namespace domain::entities
