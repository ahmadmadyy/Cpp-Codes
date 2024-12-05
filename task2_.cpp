#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <cmath>

// Class representing a player in the combat game
class Player {
    private:
        int number;
        std::string surname;
        std::string firstname;
        int height; // in cm
        double weight; // in kg
        int points;
        int year_of_birth;
        int fights;
        int wins;
        int ties;

    public:
        Player(int num, const std::string& sname, const std::string& fname, int h, double w, int yob)
            : number(num), surname(sname), firstname(fname), height(h), weight(w), points(0), year_of_birth(yob), fights(0), wins(0), ties(0) {}

        // Overloaded input operator for Player data
        friend std::istream& operator>>(std::istream& in, Player& player) {
            std::cout << "Enter player number: ";
            in >> player.number;
            std::cout << "Enter surname: ";
            in >> player.surname;
            std::cout << "Enter first name: ";
            in >> player.firstname;
            std::cout << "Enter height (cm): ";
            in >> player.height;
            std::cout << "Enter weight (kg): ";
            in >> player.weight;
            std::cout << "Enter year of birth: ";
            in >> player.year_of_birth;
            return in;
        }

        // Overloaded output operator for Player data
        friend std::ostream& operator<<(std::ostream& out, const Player& player) {
            out << player.firstname << " " << player.surname << ", " << player.year_of_birth << ", "
                << player.height << "cm, " << player.weight << "kg, participated in " << player.fights
                << " combat games: " << player.wins << " x won, " << player.ties << " x tie = "
                << player.points << " points";
            return out;
        }

        // Operator to compare players
        bool operator>=(const Player& other) const {
            return (height >= other.height && weight >= other.weight);
        }

        // Function to update fight results
        void updateResult(const std::string& result) {
            ++fights;
            if (result == "win") {
                points += 10;
                ++wins;
            } else if (result == "tie") {
                points += 5;
                ++ties;
            }
        }

        // Getters
        int getNumber() const { return number; }
        int getFights() const { return fights; }
        const std::string& getFullName() const { return firstname + " " + surname; }
        int getHeightDifference(const Player& other) const { return height - other.height; }
        double getWeightDifference(const Player& other) const { return weight - other.weight; }
};

// Class managing the combat game system
class CombatGameManager {
    private:
        std::vector<std::shared_ptr<Player>> players;

    public:
        void createPlayer() {
            if (players.size() >= 6) {
                std::cout << "Maximum number of players reached.\n";
                return;
            }

            auto player = std::make_shared<Player>(0, "", "", 0, 0.0, 0);
            std::cin >> *player;
            if (std::any_of(players.begin(), players.end(), [&](const auto& p) { return p->getNumber() == player->getNumber(); })) {
                std::cout << "Player number must be unique.\n";
            } else {
                players.push_back(player);
                std::cout << "Player added successfully.\n";
            }
        }

        void manageCombat() {
    if (players.size() < 2) {
        std::cout << "Not enough players to start a combat.\n";
        return;
    }

    int p1_num, p2_num;
    std::cout << "Enter player 1 number: ";
    std::cin >> p1_num;
    std::cout << "Enter player 2 number: ";
    std::cin >> p2_num;

    auto it1 = std::find_if(players.begin(), players.end(), [&](const auto& p) { return p->getNumber() == p1_num; });
    auto it2 = std::find_if(players.begin(), players.end(), [&](const auto& p) { return p->getNumber() == p2_num; });

    // Check if both players exist
    if (it1 == players.end() || it2 == players.end()) {
        std::cout << "One or both players not found.\n";
        return;
    }

    // Safely dereference the smart pointers
    auto& player1 = *it1;
    auto& player2 = *it2;

    // Check if either player has already participated in 3 fights
    if (player1->getFights() >= 3 || player2->getFights() >= 3) {
        std::cout << "One or both players have already participated in 3 fights.\n";
        return;
    }

    std::string result;
    std::cin.ignore();  // Clear any leftover input in the buffer
    std::cout << "Enter result (win/tie/lose) for player 1: ";
    std::cin >> result;

    // Validate result input
    if (result != "win" && result != "tie" && result != "lose") {
        std::cout << "Invalid result entered. Please enter 'win', 'tie', or 'lose'.\n";
        return;
    }

    // Update results based on the entered outcome
    if (result == "win") {
        player1->updateResult("win");
        player2->updateResult("lose");
    } else if (result == "tie") {
        player1->updateResult("tie");
        player2->updateResult("tie");
    } else {
        player1->updateResult("lose");
        player2->updateResult("win");
    }

    // Output height and weight differences along with the results
    std::cout << "Player " << player1->getFullName() << " has "
              << (player1->getHeightDifference(*player2) >= 0 ? "won" : "lost") << ", they are "
              << std::abs(player1->getHeightDifference(*player2)) << " cm "
              << (player1->getHeightDifference(*player2) >= 0 ? "taller" : "shorter") << " and "
              << std::abs(player1->getWeightDifference(*player2)) << " kg "
              << (player1->getWeightDifference(*player2) >= 0 ? "heavier" : "lighter") << ".\n";
}



        void outputAllPlayers() {
            for (const auto& player : players) {
                std::cout << *player << "\n";
            }
        }

        void outputWinner() {
            auto winner = std::max_element(players.begin(), players.end(), [](const auto& p1, const auto& p2) {
                return p1->getFights() < p2->getFights();
            });

            if (winner != players.end()) {
                std::cout << "Winner: " << **winner << "\n";
            } else {
                std::cout << "No players have participated in fights yet.\n";
            }
        }

        void run() {
            int choice;
            do {
                std::cout << "\nCombat Game Manager\n";
                std::cout << "1. Create player\n2. Manage combat game\n3. Output all players\n4. Output winner\n5. Exit\nEnter choice: ";
                std::cin >> choice;

                switch (choice) {
                    case 1:
                        createPlayer();
                        break;
                    case 2:
                        manageCombat();
                        break;
                    case 3:
                        outputAllPlayers();
                        break;
                    case 4:
                        outputWinner();
                        break;
                    case 5:
                        std::cout << "Exiting program.\n";
                        break;
                    default:
                        std::cout << "Invalid choice. Try again.\n";
                }
            } while (choice != 5);
        }
};

int main() {
    CombatGameManager manager;
    manager.run();
    return 0;
}
