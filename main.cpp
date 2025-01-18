#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
using namespace std;

// Base class for Person
class Person {
protected:
    string first_name;
    string surname;
    string email;

public:
    Person(const string& first_name, const string& surname, const string& email)
        : first_name(first_name), surname(surname), email(email) {}

    string getEmail() const {
        return email;
    }

    virtual void display() const {
        cout << "Name: " << first_name << " " << surname << ", Email: " << email << endl;
    }
};

// Derived class for Lecturer
class Lecturer : public Person {
private:
    string academic_title;

public:
    Lecturer(const string& first_name, const string& surname, const string& email, const string& academic_title)
        : Person(first_name, surname, email), academic_title(academic_title) {}

    string getAcademicTitle() const {
        return academic_title;
    }

    void display() const override {
        cout << academic_title << " " << first_name << " " << surname << ", Email: " << email << endl;
    }
};

// Derived class for Student
class Student : public Person {
private:
    string university;
    int matriculation_number;

public:
    Student(const string& first_name, const string& surname, const string& email, const string& university, int matriculation_number)
        : Person(first_name, surname, email), university(university), matriculation_number(matriculation_number) {}

    string getUniversity() const {
        return university;
    }

    void display() const override {
        Person::display();
        cout << "University: " << university << ", Matriculation Number: " << matriculation_number << endl;
    }
};

// Course class
class Course {
private:
    string name;
    Lecturer lecturer;
    vector<Student> participants;

public:
    static const int MAX_PARTICIPANTS = 10;
    static const int MIN_PARTICIPANTS = 3;

    Course(const string& name, const Lecturer& lecturer)
        : name(name), lecturer(lecturer) {}

    string getName() const {
        return name;
    }

    Lecturer getLecturer() const {
        return lecturer;
    }

    bool isFullyBooked() const {
        return participants.size() >= MAX_PARTICIPANTS;
    }

    bool addParticipant(const Student& student) {
        if (isFullyBooked()) {
            cout << "Course is already fully booked." << endl;
            return false;
        }
        if (find_if(participants.begin(), participants.end(),
                    [&student](const Student& s) { return s.getEmail() == student.getEmail(); }) != participants.end()) {
            cout << "A student with this email is already registered in this course." << endl;
            return false;
        }
        participants.push_back(student);
        return true;
    }

    void displayParticipants() const {
        cout << "Course: " << name << ", Lecturer: ";
        lecturer.display();
        if (participants.size() < MIN_PARTICIPANTS) {
            cout << "Course will not take place due to insufficient participants." << endl;
        } else {
            cout << "Participants:" << endl;
            for (const auto& participant : participants) {
                participant.display();
            }
        }
    }

    void displayAvailableSeats() const 
    {
        cout << "Course: " << name << ", Lecturer: ";
        lecturer.display();
        cout << "Available seats: " << (MAX_PARTICIPANTS - participants.size()) << endl;
    }

    bool hasFewParticipants() const 
    {
        return participants.size() < MIN_PARTICIPANTS;
    }

    const vector<Student>& getParticipants() const 
    {
        return participants;
    }
};

int main() {
    // Sample lecturers
    Lecturer lecturer1("John", "Doe", "john.doe@example.com", "Prof.");
    Lecturer lecturer2("Jane", "Smith", "jane.smith@example.com", "Dr.");
    Lecturer lecturer3("Alan", "Turing", "alan.turing@example.com", "Prof.");

    // Courses
    vector<Course> courses = {
        Course("Programming", lecturer1),
        Course("Databases", lecturer2),
        Course("Software Engineering", lecturer3)
    };

    vector<Student> students;

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

            Student new_student(first_name, surname, email, university, matriculation_number);

            cout << "Available courses:\n";
            for (size_t i = 0; i < courses.size(); ++i) {
                cout << i + 1 << ". " << courses[i].getName() << endl;
            }
            cout << "Select a course: ";
            cin >> course_index;

            if (course_index < 1 || course_index > courses.size()) {
                cout << "Invalid course selection!" << endl;
                continue;
            }

            Course& selected_course = courses[course_index - 1];
            if (university != "Our University" && new_student.getUniversity() != university) {
                cout << "Students from other universities may only take one course." << endl;
                continue;
            }

            if (selected_course.addParticipant(new_student)) {
                students.push_back(new_student);
                cout << "Registration successful!" << endl;
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
            cout << "Notifying participants of courses that will not take place:\n";
            for (const auto& course : courses) {
                if (course.hasFewParticipants()) {
                    for (const auto& participant : course.getParticipants()) {
                        participant.display();
                    }
                }
            }
            cout << "Program ended." << endl;
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
