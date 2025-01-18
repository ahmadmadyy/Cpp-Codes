/*
Initial situation: 

At a university, 3 courses are offered: "Programming", "Databases" and "Software Engineering".

Each course is offered by a lecturer.

Create for each course an instance of type COURSE (=class) using a constructor (parameters: name of the course and the lecturer).
The lecturer is stored in a data member of type LECTURER (=class). 
The class LECTURER should have at least the data member academic_title.

The courses have a maximum of 10 and a minimum of 3 course participants.
The courses can be attended not only by our own students, but also by students from other universities.

The class STUDENT should have at least the following data members: 

Matriculation number
University

Students from their own university may take any course.
Students from other universities may only take one course.

The classes LECTURER and STUDENT should derive from the class PERSON.
The class PERSON should have at least the following data members:

Surname
First name
Email
Task: 

Create a program with a menu that covers the following situation:

1. Registration for a course (including querying student data). Each student has a unique email, meaning there can't be 
two or more students with the same email in the same course.

2. Output of one/all courses with data of the participants.
    Please note that if there are fewer than 3 participants in a course, an additional message must be displayed -
    "Course will not take place".

3. Output of all courses that are not fully booked yet. The output must include the number of free places, the name 
of the course and the academic name of the lecturer with its academic title.

4. End of program: A message will be displayed stating which participants (=all data members of the class PERSON) have
 to be notified because your course is not taking place.


If necessary (creation, search, and so on), corresponding messages are to be displayed.

*/
#include <iostream>
#include <vector>
#include <string>

// Base class for Person
class Person
{
    public:
        Person(const std::string& surname, const std::string& first_name, const std::string& email) :
                 surname_(surname), first_name_(first_name), email_(email)     
        {

        }

        std::string getSurname() const { return this->surname_; }
        std::string getFirstname() const { return this->first_name_; }
        std::string getEmail() const { return this->email_; }

        virtual void display() const 
        {
            std::cout << "Name: " << this->getFirstname() << " " << this->getFirstname() << ", Email: " << this->getEmail() << std::endl;
        }

    protected:
        std::string surname_;
        std::string first_name_;
        std::string email_;
};

enum Title
{
    DR = 1U,
    ASSISS_PROF,
    ASSOC_PROF,
    PROF
};

class Lecturer : public Person
{
    public:
        Lecturer(const std::string& surname, const std::string& first_name, const std::string& email, Title academic_title) : 
                        Person(surname, first_name, email), academic_title_(academic_title)
        {

        }

        Title get_academic_title()
        {
            return this->academic_title_;
        }

        void display() const override 
        {
            std::cout << this->academic_title_ << " " << this->getFirstname() << " " << this->getSurname() << ", Email: " << this->getEmail() << std::endl;
        }

    private:
        Title academic_title_;

};

class Student : public Person
{
    public:
        Student(const std::string& surname, const std::string& first_name, const std::string& email, int marticulation_number, const std::string& university) : 
        Person(surname, first_name, email), marticulation_number_(marticulation_number), university_(university)
        {

        }

        std::string getUniversity() const { return this->university_; }
        int getMarticulationNumber() const { return this->marticulation_number_; }

        void display() const override 
        {
            Person::display();
            std::cout << "University: " << this->getUniversity() << ", Matriculation Number: " << this->getMarticulationNumber() << std::endl;
        }

    private:
        int marticulation_number_;
        std::string university_;

};

class Course
{
    public:
        Course(const std::string& name, const Lecturer& lecturer) : name_(name), lecturer_(lecturer)
        {

        }

        static const int MAX_PARTICIPANTS = 10;
        static const int MIN_PARTICIPANTS = 3;

        std::string getCourseName() const
        {
            return this->name_;
        }

        std::string getLecturerName() const
        {
            return this->lecturer_.getSurname();
        }

        std::string getName() const { return this->name_; }
        Lecturer getLecturer() const { return this->lecturer_; }
        const std::vector<Student>& getParticipants() const { return participants; }

        bool isFullyBooked() const
        {
            return participants.size() >= MAX_PARTICIPANTS;
        }

        bool addParticipant(const Student& student)
        {
            if (isFullyBooked()) 
            {
                std::cout << "Course is already fully booked." << std::endl;
                return false;
            }
            std::vector<Student>::iterator iterator_;
            for(iterator_ = participants.begin(); iterator_ != participants.end(); ++iterator_)
            {
                if(student.getEmail() == iterator_->getEmail())
                {
                    return false;
                }
            }
            participants.push_back(student);
            return true;
            std::cout << "The Student with email: " << student.getEmail() << " " << " was successfuly added! " << std::endl;
        }

        void displayParticipants() const
        {
            std::cout << "Course: " << this->getName() << " " << "Lecturer: " << this->getLecturerName() << std::endl;
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

        void displayAvailableSeats() const 
        {
            std::cout << "Course: " << this->getName() << ", Lecturer: ";
            this->lecturer_.display();
            std::cout << "Available seats: " << (MAX_PARTICIPANTS - participants.size()) << std::endl;
        }

        bool hasFewParticipants() const 
        {
            return participants.size() < MIN_PARTICIPANTS;
        }

    private:
        std::string name_;
        Lecturer lecturer_;
        std::vector<Student> participants;
};

int main()
{
    using namespace std;
    // Sample lecturers
    Lecturer lecturer1("Elon", "Musk", "elon.musk@tesla.com", PROF);
    Lecturer lecturer2("Steve", "Jobs", "steve.jobs@apple.com", DR);
    Lecturer lecturer3("Bill", "Gates", "bill.gates@microsoft.com", PROF);

    // Courses
    vector<Course> courses = 
    {
        Course("Programming", lecturer1),
        Course("Databases", lecturer2),
        Course("Software Engineering", lecturer3)
    };

    vector<Student> students;

    while (true) 
    {
        cout << "\nMenu:\n"
             << "\n1. Register for a course\n" 
             << "\n2. Display course details\n" 
             << "\n3. Display courses with available seats\n" 
             << "\n4. End program\n" 
             << "\nEnter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string first_name, surname, email, university;
            int matriculation_number, course_index;

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

            Student new_student(surname, first_name, email, matriculation_number, university);

            cout << "\nAvailable courses:\n";
            for (size_t i = 0; i < courses.size(); ++i) 
            {
                cout << i + 1 << ". " << courses[i].getName() << endl;
            }
            cout << "\nSelect a course: ";
            cin >> course_index;

            if (course_index < 1 || course_index > courses.size()) 
            {
                cout << "\nInvalid course selection!" << endl;
                continue;
            }

            Course& selected_course = courses[course_index - 1];
            if (university != "Our University" && new_student.getUniversity() != university) 
            {
                cout << "\nStudents from other universities may only take one course." << endl;
                continue;
            }

            if (selected_course.addParticipant(new_student)) 
            {
                students.push_back(new_student);
                cout << "\nRegistration successful!" << endl;
            }
        } 
        else if (choice == 2) 
        {
            for (const auto& course : courses) 
            {
                course.displayParticipants();
            }
        } 
        else if (choice == 3) {
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
            break;
        } 
        else 
        {
            cout << "\nInvalid choice! Please try again." << endl;
        }
    }

    return 0;
}

