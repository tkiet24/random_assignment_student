#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

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

std::vector<Student> loadStudents(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            students.emplace_back(line);
        }
    }
    return students;
}

std::vector<Activity> loadActivities(const std::string& filename) {
    std::vector<Activity> activities;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            // Parse format: "activity_name,category"
            size_t comma = line.find(',');
            if (comma != std::string::npos) {
                std::string name = line.substr(0, comma);
                std::string category = line.substr(comma + 1);
                activities.emplace_back(name, category);
            }
        }
    }
    return activities;
}

std::vector<Activity> getRandomActivities(const std::vector<Activity>& activities, int count) {
    if (activities.size() <= count) {
        return activities; // Return all if not enough activities
    }
    
    std::vector<Activity> result;
    std::vector<size_t> indices(activities.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    // Shuffle indices
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);
    
    // Take first 'count' activities
    for (int i = 0; i < count && i < indices.size(); ++i) {
        result.push_back(activities[indices[i]]);
    }
    
    return result;
}

int main() {
    try {
        std::cout << "Student Activity Assignment System\n";
        std::cout << "==================================\n\n";
        
        // Load data from files
        auto students = loadStudents("data/students.txt");
        auto activities = loadActivities("data/activities.txt");
        
        if (students.empty()) {
            std::cout << "No students found!\n";
            return 1;
        }
        
        if (activities.empty()) {
            std::cout << "No activities found!\n";
            return 1;
        }
        
        std::cout << "Loaded " << students.size() << " students and " 
                  << activities.size() << " activities.\n\n";
        
        // Assign random activities to each student
        for (const auto& student : students) {
            auto assignedActivities = getRandomActivities(activities, 3);
            
            std::cout << student.getId() << ": ";
            for (size_t i = 0; i < assignedActivities.size(); ++i) {
                std::cout << assignedActivities[i].getFormattedActivity();
                if (i < assignedActivities.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
        
        std::cout << "\nAssignment completed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
