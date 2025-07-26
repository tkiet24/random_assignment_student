#pragma once
#include <string>

namespace domain::entities {

class Student {
public:
    std::string id;
    
    Student(std::string studentId) : id(std::move(studentId)) {}
    
    const std::string& getId() const { return id; }
    
    bool operator==(const Student& other) const = default;
};

} // namespace domain::entities
