#include <iostream>
#include <memory>
#include <string>
#include <utility> // for std::move
#include <algorithm> // for std::find_if
#include <vector>

class Person {
public:
    // Constructor with member initializer list and move semantics
    Person(std::string surname, std::string first_name, std::string email)
        : surname_(std::move(surname)), first_name_(std::move(first_name)), email_(std::move(email)) {}

    // Deleted default constructor to ensure explicit initialization
    Person() = delete;

    // Rule of five: explicitly declare virtual destructor for polymorphic use
    virtual ~Person() = default;

    // Deleted copy constructor and copy assignment operator for immutability
    Person(const Person&) = delete;
    Person& operator=(const Person&) = delete;

    // Default move constructor and move assignment operator
    Person(Person&&) noexcept = default;
    Person& operator=(Person&&) noexcept = default;

    // Getter methods with `noexcept` for better performance guarantees
    const std::string& getSurname() const noexcept { return surname_; }
    const std::string& getFirstname() const noexcept { return first_name_; }
    const std::string& getEmail() const noexcept { return email_; }

    // Virtual method to display the Person's information
    virtual void display() const {
        std::cout << "Name: " << getFirstname() << " " << getSurname()
                  << ", Email: " << getEmail() << std::endl;
    }

protected:
    // Use `std::string` directly as members, taking advantage of its move semantics
    std::string surname_;
    std::string first_name_;
    std::string email_;
};

// Enumeration for academic titles
enum class Title {
    DR,
    ASSIST_PROF,
    ASSOC_PROF,
    PROF
};

// Derived class for Lecturer, inheriting from Person
class Lecturer : public Person {
public:
    // Constructor with member initializer list and move semantics
    Lecturer(std::string surname, std::string first_name, std::string email, Title academic_title)
        : Person(std::move(surname), std::move(first_name), std::move(email)),
          academic_title_(academic_title) {}

    // Deleted default constructor for explicit initialization
    Lecturer() = delete;

    // Rule of five: virtual destructor for proper cleanup in polymorphic use
    ~Lecturer() override = default;

    // Default move constructor and move assignment operator
    Lecturer(Lecturer&&) noexcept = default;
    Lecturer& operator=(Lecturer&&) noexcept = default;

    // Getter for academic title (const and noexcept for efficiency)
    Title getAcademicTitle() const noexcept {
        return academic_title_;
    }

    // Overridden display method to include academic title
    void display() const override {
        std::cout << getTitleString(academic_title_) << " " << getFirstname() << " " << getSurname()
                  << ", Email: " << getEmail() << std::endl;
    }

private:
    // Helper function to convert Title enum to string for display
    static std::string getTitleString(Title title) {
        switch (title) {
        case Title::DR: return "Dr.";
        case Title::ASSIST_PROF: return "Assistant Professor";
        case Title::ASSOC_PROF: return "Associate Professor";
        case Title::PROF: return "Professor";
        default: return "Unknown Title";
        }
    }

    Title academic_title_; // Lecturer's academic title
};

// Derived class for Student, inheriting from Person
class Student : public Person {
public:
    // Constructor with member initializer list and move semantics
    Student(std::string surname, std::string first_name, std::string email, int matriculation_number, std::string university)
        : Person(std::move(surname), std::move(first_name), std::move(email)),
          matriculation_number_(matriculation_number), university_(std::move(university)) {}

    // Deleted default constructor for explicit initialization
    Student() = delete;

    // Rule of five: virtual destructor for proper cleanup in polymorphic contexts
    ~Student() override = default;

    // Default move constructor and move assignment operator
    Student(Student&&) noexcept = default;
    Student& operator=(Student&&) noexcept = default;

    // Getter for university name (const and noexcept for efficiency)
    const std::string& getUniversity() const noexcept {
        return university_;
    }

    // Getter for matriculation number (noexcept for performance)
    int getMatriculationNumber() const noexcept {
        return matriculation_number_;
    }

    // Overridden display method to include university and matriculation number
    void display() const override {
        Person::display(); // Call base class display method
        std::cout << "University: " << getUniversity()
                  << ", Matriculation Number: " << getMatriculationNumber() << std::endl;
    }

private:
    int matriculation_number_; // Student's matriculation number
    std::string university_;   // Name of the university the student is enrolled in
};

class Course {
public:
    // Constructor with member initializer list and move semantics
    Course(std::string name, std::shared_ptr<Lecturer> lecturer)
        : name_(std::move(name)), lecturer_(std::move(lecturer)) {}

    // Deleted default constructor for explicit initialization
    Course() = delete;

    // Rule of five: default destructor, copy/move constructors, and assignment operators
    ~Course() = default;
    Course(const Course&) = delete;            // Delete copy constructor
    Course& operator=(const Course&) = delete; // Delete copy assignment
    Course(Course&&) noexcept = default;       // Allow move constructor
    Course& operator=(Course&&) noexcept = default; // Allow move assignment

    // Constants for maximum and minimum participants
    static constexpr int MAX_PARTICIPANTS = 10;
    static constexpr int MIN_PARTICIPANTS = 3;

    // Getter for course name (const and noexcept for efficiency)
    const std::string& getName() const noexcept { return name_; }

    // Getter for Lecturer object (returns by const reference for efficiency)
    const Lecturer& getLecturer() const noexcept { return *lecturer_; }

    // Getter for the list of participants
    const std::vector<std::unique_ptr<Student>>& getParticipants() const noexcept { return participants_; }

    // Check if the course is fully booked
    bool isFullyBooked() const noexcept { return participants_.size() >= MAX_PARTICIPANTS; }

    // Add a participant to the course
    bool addParticipant(std::unique_ptr<Student> student) {
        if (isFullyBooked()) {
            std::cout << "Course is already fully booked." << std::endl;
            return false;
        }

        // Check for duplicate email using `std::find_if`
        auto it = std::find_if(participants_.begin(), participants_.end(), 
                               [&student](const std::unique_ptr<Student>& participant) {
                                   return participant->getEmail() == student->getEmail();
                               });

        if (it != participants_.end()) {
            std::cout << "Student with email " << student->getEmail() << " is already registered." << std::endl;
            return false;
        }

        participants_.push_back(std::move(student));
        std::cout << "Student was successfully added!" << std::endl;
        return true;
    }

    // Display all participants of the course
    void displayParticipants() const {
        std::cout << "Course: " << getName() 
                  << ", Lecturer: " << lecturer_->getFirstname() << " " << lecturer_->getSurname() << std::endl;
        lecturer_->display();

        if (participants_.size() < MIN_PARTICIPANTS) {
            std::cout << "Course will not take place due to insufficient participants." << std::endl;
        } else {
            std::cout << "Participants:" << std::endl;
            for (const auto& participant : participants_) {
                participant->display();
            }
        }
    }

    // Display available seats in the course
    void displayAvailableSeats() const {
        std::cout << "Course: " << getName() << ", Lecturer: ";
        lecturer_->display();
        std::cout << "Available seats: " << (MAX_PARTICIPANTS - participants_.size()) << std::endl;
    }

    // Check if the course has fewer participants than the minimum required
    bool hasFewParticipants() const noexcept { return participants_.size() < MIN_PARTICIPANTS; }

private:
    std::string name_;                            // Name of the course
    std::shared_ptr<Lecturer> lecturer_;          // Shared pointer to Lecturer for shared ownership
    std::vector<std::unique_ptr<Student>> participants_; // Vector of unique pointers to students
};

int main() {
    using namespace std;

    // Initialize sample lecturers using shared pointers
    auto lecturer1 = make_shared<Lecturer>("Elon", "Musk", "elon.musk@tesla.com", 'PROF');
    auto lecturer2 = make_shared<Lecturer>("Steve", "Jobs", "steve.jobs@apple.com", 'DR');
    auto lecturer3 = make_shared<Lecturer>("Bill", "Gates", "bill.gates@microsoft.com", 'PROF');

    // Create courses, each associated with a shared lecturer
    vector<Course> courses = {
        Course("Programming", lecturer1),
        Course("Databases", lecturer2),
        Course("Software Engineering", lecturer3)
    };

    while (true) {
        cout << "\nMenu:\n"
             << "1. Register for a course\n"
             << "2. Display course details\n"
             << "3. Display courses with available seats\n"
             << "4. End program\n"
             << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string first_name, surname, email, university;
            int matriculation_number, course_index;

            cout << "Enter student's first name: ";
            cin >> first_name;
            cout << "Enter student's surname: ";
            cin >> surname;
            cout << "Enter student's email: ";
            cin >> email;
            cout << "Enter student's university: ";
            cin >> university;
            cout << "Enter student's matriculation number: ";
            cin >> matriculation_number;

            auto new_student = make_unique<Student>(surname, first_name, email, matriculation_number, university);

            cout << "Available courses:\n";
            for (size_t i = 0; i < courses.size(); ++i) {
                cout << i + 1 << ". " << courses[i].getName() << endl;
            }

            cout << "Select a course: ";
            cin >> course_index;

            if (course_index < 1 || course_index > courses.size()) {
                cout << "Invalid course selection!\n";
                continue;
            }

            Course& selected_course = courses[course_index - 1];

            if (selected_course.addParticipant(std::move(new_student))) {
                cout << "Registration successful!\n";
            }

        } else if (choice == 2) {
            for (const auto& course : courses) {
                course.displayParticipants();
            }
        } else if (choice == 3) {
            for (const auto& course : courses) {
                if (!course.isFullyBooked()) {
                    course.displayAvailableSeats();
                }
            }
        } else if (choice == 4) {
            cout << "\nNotifying participants of courses that will not take place:\n";
            for (const auto& course : courses) {
                if (course.hasFewParticipants()) {
                    for (const auto& participant : course.getParticipants()) {
                        participant->display();
                    }
                }
            }
            cout << "\nProgram ended.\n";
            break;
        } else {
            cout << "\nInvalid choice! Please try again.\n";
        }
    }

    return 0;
}