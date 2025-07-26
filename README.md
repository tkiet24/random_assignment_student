# Hệ thống Phân công Hoạt động Sinh viên

## Tổng quan Dự án

Đây là một chương trình C++ hiện đại được thiết kế để phân công ngẫu nhiên các hoạt động ngoại khóa cho sinh viên theo yêu cầu của bài tập môn "Lập trình hướng đối tượng". Chương trình sử dụng các kỹ thuật và tính năng mới nhất của C++17, C++20, và C++23.

## Kiến trúc và Thiết kế

### Clean Architecture (3-Layer Architecture)

Dự án được tổ chức theo mô hình Clean Architecture với 3 tầng chính:

1. **Domain Layer** (`src/domain/`): Chứa business logic core

   - `entities/`: Các thực thể nghiệp vụ (Student, Activity)
   - `repositories/`: Interfaces cho data access

2. **Application Layer** (`src/application/`): Chứa use cases và business logic

   - `services/`: Business services
   - `strategies/`: Strategy pattern implementations

3. **Infrastructure Layer** (`src/infrastructure/`): Chứa implementation details

   - `repositories/`: Concrete repository implementations
   - `utils/`: Utility classes và helper functions

4. **Presentation Layer** (`src/presentation/`): Giao diện người dùng
   - `controllers/`: Controllers xử lý input/output

### Design Patterns Được Sử Dụng

#### 1. Repository Pattern (Structural - Độ khó Trung bình)

- **Mục đích**: Tách biệt business logic khỏi data access logic
- **Implementation**:
  - `IStudentRepository` và `IActivityRepository` interfaces
  - `FileStudentRepository` và `FileActivityRepository` concrete implementations
- **Lợi ích**: Dễ dàng thay đổi cách lưu trữ dữ liệu mà không ảnh hưởng business logic

#### 2. Strategy Pattern (Behavioral - Độ khó Trung bình)

- **Mục đích**: Cho phép thay đổi thuật toán random selection tại runtime
- **Implementation**:
  - `IRandomSelectionStrategy` interface
  - `StandardRandomStrategy` và `WeightedRandomStrategy` concrete strategies
- **Lợi ích**: Flexibility trong việc chọn thuật toán và dễ dàng extend

## Các Kỹ Thuật C++ Hiện Đại Được Sử Dụng

### 1. std::optional (C++17)

```cpp
// Trong Activity.h
[[nodiscard]] static std::optional<ActivityCategory> stringToCategory(const std::string& str);

// Trong IRandomSelectionStrategy.h
[[nodiscard]] virtual std::optional<domain::entities::Activity> selectRandomActivity(...) const = 0;
```

### 2. std::variant (C++17)

```cpp
// Trong Results.h
using ApplicationError = std::variant<FileError, ValidationError, std::error_code>;
```

### 3. Structured Bindings (C++17)

```cpp
// Trong ActivityAssignmentController.h
for (const auto& [student, activities] : results) { // Structured binding
    std::cout << std::format("{}: {}, {}, {}\n", ...);
}
```

### 4. if constexpr (C++17)

```cpp
// Trong main.cpp
template<bool UseWeightedStrategy = false>
[[nodiscard]] static std::unique_ptr<...> createController() {
    if constexpr (UseWeightedStrategy) {
        strategy = application::strategies::createWeightedRandomStrategy();
    } else {
        strategy = application::strategies::createStandardRandomStrategy();
    }
}
```

### 5. Nested Namespace Definitions (C++17)

```cpp
namespace domain::entities { ... }
namespace application::strategies { ... }
namespace infrastructure::repositories { ... }
```

### 6. Concepts (C++20)

```cpp
// Trong Concepts.h
template<typename T>
concept Randomizable = requires(T t) {
    { t.begin() } -> std::forward_iterator;
    { t.end() } -> std::forward_iterator;
    { t.size() } -> std::convertible_to<std::size_t>;
};
```

### 7. Ranges (C++20)

```cpp
// Trong IRandomSelectionStrategy.h
auto filteredActivities = activities
    | std::views::filter([category](const auto& activity) {
        return activity.getCategory() == category;
    });
```

### 8. std::format (C++20)

```cpp
// Trong Activity.h
[[nodiscard]] std::string getFormattedActivity() const {
    return std::format("{} ({})", name_, categoryToString(category_));
}
```

### 9. std::expected (C++23)

```cpp
// Trong Student.h
[[nodiscard]] static std::expected<Student, StudentIdError> create(std::string id);

// Trong Results.h
template<typename T>
using Result = std::expected<T, ApplicationError>;
```

### 10. std::span (C++20)

```cpp
// Trong ActivityAssignmentService.h
[[nodiscard]] bool validateActivitiesAvailable(
    std::span<const domain::entities::Activity> activities) const noexcept;
```

## Các Tính Năng Bổ Sung

### Modern C++ Best Practices

- **RAII**: Automatic resource management
- **const correctness**: Extensive use of const và constexpr
- **noexcept specification**: Exception safety guarantees
- **[[nodiscard]]**: Prevent accidental value discarding
- **Smart pointers**: std::unique_ptr cho memory management
- **Move semantics**: Efficient object transfer

### Error Handling

- std::expected cho functional error handling
- std::variant để combine multiple error types
- Comprehensive error formatting và reporting

### Type Safety

- Strong typing với enum classes
- Concepts để enforce type constraints
- Template metaprogramming với if constexpr

## Cách Build và Chạy

### Option 1: Sử dụng CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
cmake --build .
cmake --build . --target run
```

### Option 2: Sử dụng Makefile

```bash
# For GCC/Clang
make run

# For MSVC
make msvc-run

# Using CMake through Makefile
make cmake-run
```

### Option 3: Manual Build

```bash
# GCC/Clang
g++ -std=c++23 -Wall -Wextra -I. src/main.cpp -o StudentActivityAssignment

# MSVC
cl /std:c++23 /W4 /I. src/main.cpp /Fe:StudentActivityAssignment.exe
```

## Input Files

### students.txt

```
24127000
24127001
24127002
24127003
24127004
```

### activities.txt

```
Welcome Session,Class
Charity Day,Union
Career Fair,School
Class Meeting,Class
Blood Donation,Union
Sports Festival,School
Football,Class
Singing,Union
Volunteering,School
Workshop,Class
Research,School
Drama,Union
```

## Expected Output

```
Student Activity Assignment System
==================================

Current strategy: StandardRandomStrategy

24127000: Class Meeting (Class), Charity Day (Union), Sports Festival (School)
24127001: Welcome Session (Class), Blood Donation (Union), Career Fair (School)
24127002: Football (Class), Singing (Union), Volunteering (School)
24127003: Workshop (Class), Drama (Union), Research (School)
24127004: Class Meeting (Class), Charity Day (Union), Sports Festival (School)

Assignment completed successfully!
```

## Cấu trúc Thư mục Chi tiết

```
├── src/
│   ├── domain/
│   │   ├── entities/
│   │   │   ├── Activity.h          # Activity entity với modern C++
│   │   │   └── Student.h           # Student entity với validation
│   │   └── repositories/
│   │       ├── IStudentRepository.h    # Repository interface
│   │       └── IActivityRepository.h   # Repository interface
│   ├── application/
│   │   ├── services/
│   │   │   └── ActivityAssignmentService.h  # Business logic service
│   │   └── strategies/
│   │       └── IRandomSelectionStrategy.h   # Strategy pattern
│   ├── infrastructure/
│   │   ├── repositories/
│   │   │   ├── FileStudentRepository.h      # File-based student repo
│   │   │   └── FileActivityRepository.h     # File-based activity repo
│   │   └── utils/
│   │       ├── Concepts.h          # C++20 Concepts
│   │       └── Results.h           # Error handling utilities
│   ├── presentation/
│   │   └── controllers/
│   │       └── ActivityAssignmentController.h  # Presentation layer
│   └── main.cpp                    # Application entry point
├── students.txt                    # Input data
├── activities.txt                  # Input data
├── CMakeLists.txt                  # CMake build configuration
├── Makefile                        # Alternative build system
└── README.md                       # This file
```

## Compiler Requirements

- **GCC**: Version 12+ (for C++23 support)
- **Clang**: Version 15+ (for C++23 support)
- **MSVC**: Visual Studio 2022 17.4+ (for C++23 support)

## Tính Năng Nổi Bật

1. **Type Safety**: Sử dụng strong typing và concepts
2. **Error Handling**: Modern functional error handling với std::expected
3. **Memory Safety**: RAII và smart pointers
4. **Performance**: Move semantics và zero-cost abstractions
5. **Maintainability**: Clean architecture và separation of concerns
6. **Extensibility**: Strategy pattern cho easy extension
7. **Testability**: Dependency injection và interface segregation

Dự án này thể hiện việc áp dụng comprehensive các kỹ thuật C++ hiện đại trong một ứng dụng thực tế với kiến trúc sạch và các design patterns phù hợp.
