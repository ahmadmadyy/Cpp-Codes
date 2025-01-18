#include <iostream>
#include <string>
#include <memory>

class ListOfUsers;

class User 
{
    private:
        int age;
        std::string name;
        std::shared_ptr<User> next;

    public:
        User() : age(0), name(""), next(nullptr) {}
        friend class ListOfUsers;
};

class ListOfUsers 
{
    private:
        std::shared_ptr<User> head;

    public:
        ListOfUsers() : head(nullptr) {}

        void insert_front() {
            std::shared_ptr<User> newUser = std::make_shared<User>();
            std::cout << "Enter the name: ";
            std::cin >> newUser->name;
            std::cout << "Enter the age: ";
            std::cin >> newUser->age;
            newUser->next = head;
            head = newUser;
            std::cout << "User added successfully at the front of the list." << std::endl;
        }

        void printList() {
            std::shared_ptr<User> current = head;
            while (current) {
                std::cout << "Name: " << current->name << ", Age: " << current->age << std::endl;
                current = current->next;
            }
        }
};

int main() 
{
    ListOfUsers MyList;
    std::cout << "Add the first user:" << std::endl;
    MyList.insert_front();
    std::cout << "Add the second user:" << std::endl;
    MyList.insert_front();
    std::cout << "Displaying the list of users:" << std::endl;
    MyList.printList();
    return 0;
}
