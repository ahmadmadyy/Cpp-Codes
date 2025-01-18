#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
#include <map>
#include <algorithm>
using namespace std;

// Class to manage individual Player data
class Player {
private:
    int number;               // Unique player number
    string surname;           // Player's surname
    string firstname;         // Player's first name
    int height;               // Player's height in cm
    float weight;             // Player's weight in kg
    int points;               // Player's total points
    int yearOfBirth;          // Player's year of birth
    int fightsParticipated;   // Number of fights participated in
    int wins, ties, losses;   // Number of wins, ties, and losses

public:
    // Default constructor to initialize all values
    Player() : number(0), height(0), weight(0), points(0), yearOfBirth(0),
               fightsParticipated(0), wins(0), ties(0), losses(0) {}

    // Input stream operator to read player data
    friend istream& operator>>(istream& in, Player& player) {
        cout << "Enter Player Number (unique): ";
        in >> player.number;  // Input player number
        cout << "Enter Surname: ";
        in >> player.surname;  // Input player surname
        cout << "Enter First Name: ";
        in >> player.firstname; // Input player first name
        cout << "Enter Height (cm): ";
        in >> player.height;    // Input player height
        cout << "Enter Weight (kg): ";
        in >> player.weight;    // Input player weight
        cout << "Enter Year of Birth: ";
        in >> player.yearOfBirth; // Input player year of birth
        return in;
    }

    // Output stream operator to display player information
    friend ostream& operator<<(ostream& out, const Player& player) {
        out << player.firstname << " " << player.surname << ", " << player.yearOfBirth
            << ", " << player.height << "cm, " << player.weight << "kg, participated in "
            << player.fightsParticipated << " combat games: "
            << player.wins << " x won, " << player.ties << " x tie, "
            << player.losses << " x lost = " << player.points << " points.";
        return out;
    }

    // Comparison operator to compare players based on height and weight
    bool operator>=(const Player& other) const {
        return height >= other.height && weight >= other.weight;
    }

    // Function to add the result of a fight to the player’s statistics
    void addFightResult(int result) {
        fightsParticipated++;
        if (result == 10) wins++;       // Win result
        else if (result == 5) ties++;   // Tie result
        else losses++;                  // Loss result
        points += result;               // Add points based on result
    }

    // Getter functions to access player data
    int getNumber() const { return number; }
    string getName() const { return firstname + " " + surname; }
    int getHeight() const { return height; }
    int getPoints() const { return points; }
    float getWeight() const { return weight; }
};

// Class to manage the combat game system and players
class CombatGameManager {
private:
    map<int, shared_ptr<Player>> players; // Map to store players by their unique number

    // Function to find a player by their number
    shared_ptr<Player> findPlayer(int number) const {
        auto it = players.find(number);
        return (it != players.end()) ? it->second : nullptr;
    }

public:
    // Function to create a new player
    void createPlayer() {
        if (players.size() >= 6) {
            cout << "Maximum players reached!\n";  // Limit of 6 players
            cout << endl;
            return;
        }
        auto player = make_shared<Player>();  // Create a shared pointer for the player
        cin >> *player;  // Input player data
        if (players.count(player->getNumber())) {  // Check if the player already exists
            cout << "Player with this number already exists. Try again.\n";
            cout << endl;
        } else {
            players[player->getNumber()] = player;  // Add player to the map
            cout << "Player created successfully.\n";
            cout << endl;
        }
    }

    // Function to manage a combat game between two players
    void manageCombatGame() {
        int player1Num, player2Num;
        cout << "Enter the number of Player 1: ";
        cout << endl;
        cin >> player1Num;  // Input Player 1 number
        cout << "Enter the number of Player 2: ";
        cout << endl;
        cin >> player2Num;  // Input Player 2 number

        auto player1 = findPlayer(player1Num);  // Find Player 1
        auto player2 = findPlayer(player2Num);  // Find Player 2

        if (!player1 || !player2) {
            cout << "Invalid player numbers. Try again.\n";  // Handle invalid player numbers
            return;
        }

        cout << "Enter the result of the game:\n";
        cout << endl;
        cout << "1. Player 1 wins\n";
        cout << endl;
        cout << "2. Player 2 wins\n";
        cout << endl;
        cout << "3. Tie\n";
        cout << endl;
        int result;
        cin >> result;  // Input the result of the game

        // Update player statistics based on the game result
        if (result == 1) {
            player1->addFightResult(10);  // Player 1 wins
            player2->addFightResult(0);   // Player 2 loses
            cout << player1->getName() << " has won the game.\n";
            cout << "Comparison: " << (player1->getHeight() - player2->getHeight()) << " cm taller and "
                 << (player1->getWeight() - player2->getWeight()) << " kg heavier.\n";
        } else if (result == 2) {
            player2->addFightResult(10);  // Player 2 wins
            player1->addFightResult(0);   // Player 1 loses
            cout << player2->getName() << " has won the game.\n";
            cout << "Comparison: " << (player2->getHeight() - player1->getHeight()) << " cm taller and "
                 << (player2->getWeight() - player1->getWeight()) << " kg heavier.\n";
        } else if (result == 3) {
            player1->addFightResult(5);  // Tie result for both players
            player2->addFightResult(5);
            cout << "The game is a tie.\n";
        } else {
            cout << "Invalid result entered. Try again.\n";  // Handle invalid result
        }
    }

    // Function to display all players
    void displayAllPlayers() const {
        if (players.empty()) {
            cout << "No players available.\n";  // Handle empty player list
            return;
        }
        for (const auto& pair : players) {
            cout << *pair.second << endl;  // Display each player
        }
    }

    // Function to display the winner based on points
    void displayWinner() const {
        if (players.empty()) {
            cout << "No players available.\n";  // Handle empty player list
            return;
        }

        // Find the player with the highest points
        auto winner = max_element(players.begin(), players.end(),
                                  [](const auto& a, const auto& b) {
                                      return a.second->getPoints() < b.second->getPoints();
                                  });

        cout << "Winner: " << *winner->second << endl;  // Display the winner
        cout << endl;
    }
};

// Main function to run the program
int main() {
    CombatGameManager manager;  // Create an instance of the combat game manager
    int choice;

    do {
        // Display the main menu
        cout << "\nCombat Game Management System\n";
        cout << endl;
        cout << "1. Create Player\n";
        cout << endl;
        cout << "2. Manage Combat Game\n";
        cout << endl;
        cout << "3. Display All Players\n";
        cout << endl;
        cout << "4. Display Winner\n";
        cout << endl;
        cout << "5. Exit\n";
        cout << endl;
        cout << "Enter your choice: ";
        cout << endl;
        cin >> choice;  // Input the user's choice

        // Perform the action based on user's choice
        switch (choice) {
            case 1:
                manager.createPlayer();  // Create a new player
                break;
            case 2:
                manager.manageCombatGame();  // Manage a combat game
                break;
            case 3:
                manager.displayAllPlayers();  // Display all players
                break;
            case 4:
                manager.displayWinner();  // Display the winner
                break;
            case 5:
                cout << "Exiting program. Goodbye!\n";  // Exit the program
                break;
            default:
                cout << "Invalid choice. Try again.\n";  // Handle invalid input
        }
    } while (choice != 5);  // Repeat until the user chooses to exit

    return 0;
}
