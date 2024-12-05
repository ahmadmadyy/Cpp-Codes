#include <iostream>
#include <vector>
#include <string>

// Base class for Person
class Person
{
    public:
        // Constructor to initialize a Person with surname, first name, and email
        Person(const std::string& surname, const std::string& first_name, const std::string& email) :
                 surname_(surname), first_name_(first_name), email_(email)     
        {
        }

        // Getter for surname
        std::string getSurname() const { return this->surname_; }
        
        // Getter for first name
        std::string getFirstname() const { return this->first_name_; }

        // Getter for email
        std::string getEmail() const { return this->email_; }

        // Virtual method to display the Person's information
        virtual void display() const 
        {
            std::cout << "Name: " << this->getFirstname() << " " << this->getSurname() 
                      << ", Email: " << this->getEmail() << std::endl;
        }

    protected:
        std::string surname_; // Person's surname
        std::string first_name_; // Person's first name
        std::string email_; // Person's email address
};

// Enumeration for academic titles of lecturers
enum Title
{
    DR = 1U,
    ASSISS_PROF,
    ASSOC_PROF,
    PROF
};

// Derived class for Lecturer, inheriting from Person
class Lecturer : public Person
{
    public:
        // Constructor to initialize a Lecturer with surname, first name, email, and academic title
        Lecturer(const std::string& surname, const std::string& first_name, const std::string& email, Title academic_title) : 
                        Person(surname, first_name, email), academic_title_(academic_title)
        {
        }

        // Getter for academic title
        Title get_academic_title()
        {
            return this->academic_title_;
        }

        // Overridden display method to include academic title
        void display() const override 
        {
            std::cout << this->academic_title_ << " " << this->getFirstname() << " " << this->getSurname() 
                      << ", Email: " << this->getEmail() << std::endl;
        }

    private:
        Title academic_title_; // Lecturer's academic title
};

// Derived class for Student, inheriting from Person
class Student : public Person
{
    public:
        // Constructor to initialize a Student with additional matriculation number and university
        Student(const std::string& surname, const std::string& first_name, const std::string& email, int marticulation_number, const std::string& university) : 
        Person(surname, first_name, email), marticulation_number_(marticulation_number), university_(university)
        {
        }

        // Getter for university name
        std::string getUniversity() const { return this->university_; }
        
        // Getter for matriculation number
        int getMarticulationNumber() const { return this->marticulation_number_; }

        // Overridden display method to include university and matriculation number
        void display() const override 
        {
            Person::display();
            std::cout << "University: " << this->getUniversity() 
                      << ", Matriculation Number: " << this->getMarticulationNumber() << std::endl;
        }

    private:
        int marticulation_number_; // Student's matriculation number
        std::string university_; // Name of the university the student is enrolled in
};

// Class for a Course
class Course
{
    public:
        // Constructor to initialize a Course with a name and a Lecturer
        Course(const std::string& name, const Lecturer& lecturer) : name_(name), lecturer_(lecturer)
        {
        }

        // Constants for maximum and minimum participants in a course
        static const int MAX_PARTICIPANTS = 10;
        static const int MIN_PARTICIPANTS = 3;

        // Getter for course name
        std::string getCourseName() const
        {
            return this->name_;
        }

        // Getter for the lecturer's name
        std::string getLecturerName() const
        {
            return this->lecturer_.getSurname();
        }

        // Getter for course name
        std::string getName() const { return this->name_; }
        
        // Getter for the Lecturer object
        Lecturer getLecturer() const { return this->lecturer_; }
        
        // Getter for the list of participants
        const std::vector<Student>& getParticipants() const { return participants; }

        // Check if the course is fully booked
        bool isFullyBooked() const
        {
            return participants.size() >= MAX_PARTICIPANTS;
        }

        // Add a participant to the course
        bool addParticipant(const Student& student)
        {
            // Check if the course is already fully booked
            if (isFullyBooked()) 
            {
                std::cout << "Course is already fully booked." << std::endl;
                return false;
            }
            
            // Ensure no duplicate email exists for participants
            for(const auto& participant : participants)
            {
                if(student.getEmail() == participant.getEmail())
                {
                    return false;
                }
            }
            
            // Add the student to the participants list
            participants.push_back(student);
            std::cout << "The Student with email: " << student.getEmail() 
                      << " was successfully added! " << std::endl;
            return true;
        }

        // Display all participants of the course
        void displayParticipants() const
        {
            std::cout << "Course: " << this->getName() 
                      << " Lecturer: " << this->getLecturerName() << std::endl;
            this->lecturer_.display();

            if (participants.size() < MIN_PARTICIPANTS) 
            {
                std::cout << "Course will not take place due to insufficient participants." << std::endl;
            } 
            else 
            {
                std::cout << "Participants:" << std::endl;
                for (const auto& participant : participants)
                {
                    participant.display();
                }
            }
        }

        // Display available seats in the course
        void displayAvailableSeats() const 
        {
            std::cout << "Course: " << this->getName() << ", Lecturer: ";
            this->lecturer_.display();
            std::cout << "Available seats: " << (MAX_PARTICIPANTS - participants.size()) << std::endl;
        }

        // Check if the course has fewer participants than the minimum required
        bool hasFewParticipants() const 
        {
            return participants.size() < MIN_PARTICIPANTS;
        }

    private:
        std::string name_; // Name of the course
        Lecturer lecturer_; // Lecturer for the course
        std::vector<Student> participants; // List of participants in the course
};

// Main function: Entry point of the program
int main()
{
    using namespace std;

    // Initialize sample lecturers with their details
    Lecturer lecturer1("Elon", "Musk", "elon.musk@tesla.com", PROF);
    Lecturer lecturer2("Steve", "Jobs", "steve.jobs@apple.com", DR);
    Lecturer lecturer3("Bill", "Gates", "bill.gates@microsoft.com", PROF);

    // Create a list of courses, each associated with a lecturer
    vector<Course> courses = 
    {
        Course("Programming", lecturer1),
        Course("Databases", lecturer2),
        Course("Software Engineering", lecturer3)
    };

    // List to keep track of all registered students
    vector<Student> students;

    // Menu-driven loop to interact with the program
    while (true) 
    {
        // Display menu options
        cout << "\nMenu:\n"
             << "\n1. Register for a course\n" 
             << "\n2. Display course details\n" 
             << "\n3. Display courses with available seats\n" 
             << "\n4. End program\n" 
             << "\nEnter your choice: ";
        int choice;
        cin >> choice; // Get user choice

        if (choice == 1) {
            // Handle course registration
            string first_name, surname, email, university;
            int matriculation_number, course_index;

            // Gather student details
            cout << "\nEnter student's first name: ";
            cin >> first_name;
            cout << "\nEnter student's surname: ";
            cin >> surname;
            cout << "\nEnter student's email: ";
            cin >> email;
            cout << "\nEnter student's university: ";
            cin >> university;
            cout << "\nEnter student's matriculation number: ";
            cin >> matriculation_number;

            // Create a new student object
            Student new_student(surname, first_name, email, matriculation_number, university);

            // Display available courses
            cout << "\nAvailable courses:\n";
            for (size_t i = 0; i < courses.size(); ++i) 
            {
                cout << i + 1 << ". " << courses[i].getName() << endl;
            }

            // Select a course for the student
            cout << "\nSelect a course: ";
            cin >> course_index;

            // Validate course selection
            if (course_index < 1 || course_index > courses.size()) 
            {
                cout << "\nInvalid course selection!" << endl;
                continue;
            }

            // Get the selected course
            Course& selected_course = courses[course_index - 1];

            // Check if the student is allowed to register
            if (university != "Our University" && new_student.getUniversity() != university) 
            {
                cout << "\nStudents from other universities may only take one course." << endl;
                continue;
            }

            // Add the student to the selected course
            if (selected_course.addParticipant(new_student)) 
            {
                students.push_back(new_student); // Add to the global student list
                cout << "\nRegistration successful!" << endl;
            }
        } 
        else if (choice == 2) 
        {
            // Display details of all courses and their participants
            for (const auto& course : courses) 
            {
                course.displayParticipants();
            }
        } 
        else if (choice == 3) 
        {
            // Display courses with available seats
            for (const auto& course : courses) 
            {
                if (!course.isFullyBooked()) 
                {
                    course.displayAvailableSeats();
                }
            }
        } 
        else if (choice == 4) 
        {
            // Notify participants of courses that will not take place
            cout << "\nNotifying participants of courses that will not take place:\n";
            for (const auto& course : courses) 
            {
                if (course.hasFewParticipants()) 
                {
                    for (const auto& participant : course.getParticipants()) 
                    {
                        participant.display();
                    }
                }
            }
            cout << "\nProgram ended." << endl;
            break; // Exit the loop and end the program
        } 
        else 
        {
            // Handle invalid menu choices
            cout << "\nInvalid choice! Please try again." << endl;
        }
    }

    return 0; // Indicate successful program termination
}
