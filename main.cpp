#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <memory>

const int TimeSleep = 2000;


class Player {
private:
    int gearscore;
    std::string nickname;
    std::string archetype;
    std::string rank = "solo";

public:

    Player(int g, std::string n, std::string a)
    : gearscore(g), nickname(std::move(n)), archetype(std::move(a)) {}

    int getGearscore() const {return gearscore;}
    const std::string& getNickname() const {return nickname;}
    const std::string& getArchetype() const {return archetype;}
    const std::string& getRank() const {return rank;}

    void setRank(std::string new_rank) {
        if (new_rank != "solo" && new_rank != "member" && new_rank != "officer" && new_rank != "GM") {
            std::cerr << "Error: invalid rank " << new_rank << "'" << std::endl;
            return;
        }
        rank = std::move(new_rank);
    }


    void showDetailed () const {
        std::cout << "=============" << std::endl;
        std::cout << "Nickname: " << nickname << std::endl;
        std::cout << "Gearscore: " << gearscore << std::endl;
        std::cout << "Archetype: " << archetype << std::endl;
        std::cout << "Rank: " << rank << std::endl;
    }

    bool isStrongEnough() const {
        return gearscore >= 8000;
    }
};


class Guild {
public:
    std::vector<Player> players;
    std::string name;
    int level;
    Guild(std::string n, int lvl) 
    : name(std::move(n)), level(lvl) {}

    void saveGuild() const {
        std::ofstream file("guild.txt");
        if (!file) {
            std::cout << "Error opening file for writing!" << std::endl;
            return;
        }
        file << "GuildName:" << name << std::endl;
        file << "GuildLevel:" << level << std::endl;
        for (int i = 0; i < players.size(); ++i) {
            file << "Player:" << players[i].getNickname() << players[i].getGearscore() << players[i].getArchetype() << std::endl;
        }
        file.close();
    }

    void showCount() const {
        std::cout << "Players in guild: " << size(players) << std::endl;
    }

    // Find by name
    // void findPlayer() const {
    //     std::string nickname;
    //     std::cout << "Enter player name: ";
    //     std::cin >> nickname;
    //     bool found = false;
    //     for (const Player& player : players) {
    //         if (player.getNickname() == nickname) {
    //             player.showDetailed();
    //             found = true;
    //             break;
    //         }
    //     }
    //     if (!found) {
    //         std::cout << "Player not found" << std::endl;
    //     }
    // }

    // Remove_if with lambda
    // void deletePlayer() {
    //     std::string nickname;
    //     std::cout << "Enter player name: ";
    //     std::cin >> nickname;
    //     players.erase(
    //         remove_if(players.begin(),players.end(),
    //             [&nickname](const Player& p) {
    //                 return p.getNickname() == nickname;
    //             }
    //         ),
    //     players.end()
    //     );
    //     std::cout << "Player removed" << std::endl;
    // }

    auto showAllNickname() const {
        for (auto i = 0; i < players.size(); ++i) {
            std::cout << i+1 << ". ";
            std::cout << players[i].getNickname() << std::endl;
        }
        auto choice = size_t(0);
        std::cout << "Enter player number: ";
        std::cin >> choice;
        return choice-1;
    }

    void showAllPlayersDetailed() const {
        if (checkEmptyGuild())
            return;
        std::cout << "Guild \"" << name << "\" players:" << std::endl;
        for (size_t i = 0; i < size(players); ++i) {
            std::cout << i + 1 << ". ";
            players[i].showDetailed();
        }
        showCount();
        Sleep(TimeSleep);
    }

    void deletePlayerFromGuild(std::vector <Player>& free_players) {
        if (checkEmptyGuild())
            return;
        size_t choice = showAllNickname();
        free_players.push_back(std::move(players[choice]));
        players.erase(players.begin() + choice);
        free_players.back().setRank("solo");
        std::cout << "Player " << free_players[free_players.size()-1].getNickname() << " removed!" << std::endl;
        Sleep(TimeSleep);
    }

    bool checkEmptyGuild() const {
        if (players.empty()) {
            std::cout << "No players in guild" << std::endl;
            Sleep(TimeSleep);
            return true;
        }
        return false;
    }

    void sortByGearscore() {
        if (checkEmptyGuild())
            return;
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.getGearscore() > b.getGearscore();
        });
    }

    void statsGuild () const {
        auto totalGearscore = transform_reduce(
            players.begin(),
            players.end(),
            0,
            std::plus<>(),
            [](const Player& p) {return p.getGearscore();}
        );

        std::cout << "Average GS: " << totalGearscore/players.size() << std::endl;
        std::cout << "Best GS: " << players[0].getNickname() << std::endl;
        std::cout << "Worst GS: " << players[players.size() - 1].getNickname() << std::endl;
    }

    void readyForRaid() const {
        int count = 0;
        for (const auto& p : players) {
            if (p.isStrongEnough()) {
                count += 1;
                std::cout << p.getNickname() << " ready for raid" << std::endl;
            }
        }
        std::cout << "Total players ready for raid: " << count << std::endl;
    }

    void inviteFreePlayers(std::vector <Player>& free_players) {
        system("cls");
        std::cout << "Free players:" << std::endl;
        for (int i = 0; i < size(free_players); i++) {
            std::cout << i + 1 << ". ";
            free_players[i].showDetailed();
        }

        int playerChoice;
        std::cout << "Select player number: ";
        std::cin >> playerChoice;

        if (playerChoice > 0 && playerChoice <= free_players.size()) {
            players.push_back(std::move(free_players[playerChoice - 1]));

            players.back().setRank("member");

            free_players.erase(free_players.begin() + playerChoice - 1);

            std::cout << "Player added to guild!" << std::endl;
            Sleep(TimeSleep);
        }
    }
};

// void validationChoice() {
//     int choice;
//     while (!(std::cin >> choice)) {
//         std::cout << "Error! Enter a number: ";
//         std::cin.clear();
//         std::cin.ignore(10000, '\n');
//     }
// }
// WIP...

void menu(std::unique_ptr<Guild>& guild, std::vector<Player>& free_players) {
    int choice;
    std::cout << "================================" << std::endl;
    std::cout << "1. Create guild" << std::endl;
    std::cout << "2. Add player to guild" << std::endl;
    std::cout << "3. Show all players" << std::endl;
    std::cout << "4. Remove player from guild" << std::endl;
    std::cout << "5. Sort by GS" << std::endl;
    std::cout << "6. Guild statistics" << std::endl;
    std::cout << "7. Show players ready for raid" << std::endl;
    std::cout << "8. Change player rank" << std::endl;
    std::cout << "9. Save guild to file" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Choose action: ";
    std::cin >> choice;
    if (choice == 1) {
        system("cls");
        std::string name;
        std::cout << "Enter guild name: ";
        std::cin >> name;

        guild =  std::make_unique<Guild>(name, 1);

        system("cls");
        std::cout << "Guild " << name << " created!" << std::endl;
        Sleep(TimeSleep);
    }

    if (choice == 2) {
        if (!guild) {
            system("cls");
            std::cout << "Create a guild first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }

        if (free_players.empty()) {
            std::cout << "No free players!" << std::endl;
            Sleep(TimeSleep);
            return;
        }

        guild->inviteFreePlayers(free_players);
    }

    if (choice == 3) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        guild->showAllPlayersDetailed();
    }

    if (choice == 4) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        guild->deletePlayerFromGuild(free_players);

    }

    if (choice == 5) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        guild->sortByGearscore();
        std::cout << "Players sorted!" << std::endl;
        Sleep(TimeSleep);
    }

    if (choice == 6) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        guild->statsGuild();
        Sleep(TimeSleep);
    }

    if (choice == 7) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        guild->sortByGearscore();
        guild->readyForRaid();
        Sleep(TimeSleep);
    }
    

    if (choice == 8) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        if (guild->checkEmptyGuild())
            return;
        auto choicePlayer = guild->showAllNickname();
        system("cls");
        std::cout << "1. Member" << std::endl;
        std::cout << "2. Officer" << std::endl;
        std::cout << "3. Guild Master" << std::endl;
        std::cout << "Choose rank: ";
        int choiceRank;
        std::cin >> choiceRank;

        const std::string ranks[] = {"member", "officer", "GM"};

        if (choiceRank > 1 && choiceRank < 4 ) {
            guild->players[choicePlayer].setRank(ranks[choiceRank-1]);
            std::cout << "Rank changed!" << std::endl;
        } else
            std::cout << "Invalid choice!" << std::endl;
    }


    if (choice == 9) {
        system("cls");
        if (!guild) {
            std::cout << "You don't have a guild\nCreate one first!" << std::endl;
            Sleep(TimeSleep);
            return;
        }
        std::cout << "Saving..." << std::endl;
        guild->saveGuild();
        Sleep(TimeSleep);
        std::cout << "Done!" << std::endl;
        Sleep(TimeSleep);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::vector<Player> free_players;
    free_players.emplace_back(8500, "Cuc", "healer");
    free_players.emplace_back(9300, "Satoru", "melee");
    free_players.emplace_back(10000, "Aki", "mage");

    std::unique_ptr<Guild> guild = nullptr;

    bool isRunning = true;
    while (isRunning) {
        system("cls");
        std::cout << "Guild Manager 0.1v" << std::endl;

        if (guild != nullptr) {
            std::cout << "Current guild: " << guild->name << std::endl;
            guild->showCount();
        }

        menu(guild, free_players);
    }

    return 0;
}