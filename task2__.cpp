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
    int number;
    string surname;
    string firstname;
    int height;
    float weight;
    int points;
    int yearOfBirth;
    int fightsParticipated;
    int wins, ties, losses;

public:
    Player() : number(0), height(0), weight(0), points(0), yearOfBirth(0),
               fightsParticipated(0), wins(0), ties(0), losses(0) {}

    friend istream& operator>>(istream& in, Player& player) {
        cout << "Enter Player Number (unique): ";
        in >> player.number;
        cout << "Enter Surname: ";
        in >> player.surname;
        cout << "Enter First Name: ";
        in >> player.firstname;
        cout << "Enter Height (cm): ";
        in >> player.height;
        cout << "Enter Weight (kg): ";
        in >> player.weight;
        cout << "Enter Year of Birth: ";
        in >> player.yearOfBirth;
        return in;
    }

    friend ostream& operator<<(ostream& out, const Player& player) {
        out << player.firstname << " " << player.surname << ", " << player.yearOfBirth
            << ", " << player.height << "cm, " << player.weight << "kg, participated in "
            << player.fightsParticipated << " combat games: "
            << player.wins << " x won, " << player.ties << " x tie, "
            << player.losses << " x lost = " << player.points << " points.";
        return out;
    }

    bool operator>=(const Player& other) const {
        return height >= other.height && weight >= other.weight;
    }

    void addFightResult(int result) {
        fightsParticipated++;
        if (result == 10) wins++;
        else if (result == 5) ties++;
        else losses++;
        points += result;
    }

    int getNumber() const { return number; }
    string getName() const { return firstname + " " + surname; }
    int getHeight() const { return height; }
    int getPoints() const { return points; }
    float getWeight() const { return weight; }
};

// Class to manage the combat game system
class CombatGameManager {
private:
    map<int, shared_ptr<Player>> players;

    shared_ptr<Player> findPlayer(int number) const {
        auto it = players.find(number);
        return (it != players.end()) ? it->second : nullptr;
    }

public:
    void createPlayer() {
        if (players.size() >= 6) {
            cout << "Maximum players reached!\n";
            cout << endl;
            return;
        }
        auto player = make_shared<Player>();
        cin >> *player;
        if (players.count(player->getNumber())) {
            cout << "Player with this number already exists. Try again.\n";
            cout << endl;
        } else {
            players[player->getNumber()] = player;
            cout << "Player created successfully.\n";
            cout << endl;
        }
    }

    void manageCombatGame() {
        int player1Num, player2Num;
        cout << "Enter the number of Player 1: ";
        cout << endl;
        cin >> player1Num;
        cout << "Enter the number of Player 2: ";
        cout << endl;
        cin >> player2Num;

        auto player1 = findPlayer(player1Num);
        auto player2 = findPlayer(player2Num);

        if (!player1 || !player2) {
            cout << "Invalid player numbers. Try again.\n";
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
        cin >> result;

        if (result == 1) {
            player1->addFightResult(10);
            player2->addFightResult(0);
            cout << player1->getName() << " has won the game.\n";
            cout << "Comparison: " << (player1->getHeight() - player2->getHeight()) << " cm taller and "
                 << (player1->getWeight() - player2->getWeight()) << " kg heavier.\n";
        } else if (result == 2) {
            player2->addFightResult(10);
            player1->addFightResult(0);
            cout << player2->getName() << " has won the game.\n";
            cout << "Comparison: " << (player2->getHeight() - player1->getHeight()) << " cm taller and "
                 << (player2->getWeight() - player1->getWeight()) << " kg heavier.\n";
        } else if (result == 3) {
            player1->addFightResult(5);
            player2->addFightResult(5);
            cout << "The game is a tie.\n";
        } else {
            cout << "Invalid result entered. Try again.\n";
        }
    }

    void displayAllPlayers() const {
        if (players.empty()) {
            cout << "No players available.\n";
            return;
        }
        for (const auto& pair : players) {
            cout << *pair.second << endl;
        }
    }

    void displayWinner() const {
        if (players.empty()) {
            cout << "No players available.\n";
            return;
        }

        auto winner = max_element(players.begin(), players.end(),
                                  [](const auto& a, const auto& b) {
                                      return a.second->getPoints() < b.second->getPoints();
                                  });

        cout << "Winner: " << *winner->second << endl;
        cout << endl;
    }
};

int main() {
    CombatGameManager manager;
    int choice;

    do {
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
        cin >> choice;

        switch (choice) {
            case 1:
                manager.createPlayer();
                break;
            case 2:
                manager.manageCombatGame();
                break;
            case 3:
                manager.displayAllPlayers();
                break;
            case 4:
                manager.displayWinner();
                break;
            case 5:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
