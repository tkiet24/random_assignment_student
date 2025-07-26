#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string id;
    Student(std::string i) : id(std::move(i)) {}
    const std::string& getId() const { return id; }
};

struct Activity {
    std::string name;
    std::string category;
    Activity(std::string n, std::string c) : name(std::move(n)), category(std::move(c)) {}
    std::string getFormattedActivity() const { return name + " (" + category + ")"; }
};

int main() {
    // Hardcoded test data
    std::vector<Student> students = {
        Student("24127000"),
        Student("24127001"), 
        Student("24127002")
    };
    
    std::vector<Activity> activities = {
        Activity("Class Meeting", "Class"),
        Activity("Charity Day", "Union"),
        Activity("Sports Festival", "School"),
        Activity("Workshop", "Class"),
        Activity("Drama", "Union"),
        Activity("Career Fair", "School")
    };
    
    std::cout << "Student Activity Assignment System\n";
    std::cout << "==================================\n\n";
    
    // Simple assignment - first 3 activities to first student
    for (const auto& student : students) {
        std::cout << student.getId() << ": "
                  << activities[0].getFormattedActivity() << ", "
                  << activities[1].getFormattedActivity() << ", "
                  << activities[2].getFormattedActivity() << "\n";
    }
    
    std::cout << "\nAssignment completed successfully!\n";
    return 0;
}
