#include "Student.h"

namespace domain::entities {

Student::Student(std::string studentId) : id(std::move(studentId)) {}

const std::string& Student::getId() const {
    return id;
}

} // namespace domain::entities
