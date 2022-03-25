#include <iostream>

#include "ai.hpp"
#include "simulator.hpp"

constexpr unsigned int ROUND_COUNT = 100;

int main(int argc, char* argv[]) {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
        {"a", Simulator::Snake(Simulator::Position{1, 1}, 3, 100)},
        {"b", Simulator::Snake(Simulator::Position{1, 9}, 3, 100)},
        {"c", Simulator::Snake(Simulator::Position{9, 1}, 3, 100)},
        {"d", Simulator::Snake(Simulator::Position{9, 9}, 3, 100)},
    };

    const std::unordered_map<std::string, float> ucbConstants {
        {"a", 0.25f},
        {"b", 0.50f},
        {"c", 0.75f},
        {"d", 1.00f}
    };

    std::unordered_map<std::string, unsigned int> winCounts;

    for (unsigned int i = 0; i < ROUND_COUNT; i++) {
        std::cout << "ROUND " << i << " START\n";
        const Simulator::FoodGrid food = {Grid<bool>(11, 11), 0};
        Simulator::Board board{snakes, food};

        std::cout << board.to_string();
        while (!board.is_game_over()) {
            std::unordered_map<std::string, Simulator::Direction> moveMap;
            for (const auto&[id, s]: board.get_snakes()) {

                const Simulator::Direction move = AI::mcts_suct_player(board, id, {200, ucbConstants.at(id)});
                std::cout << id << " chose '" << Simulator::direction_to_string(move) << "'\n";
                moveMap[id] = move;
            }
            std::cout << '\n';

            board.update(moveMap);
            std::cout << board.to_string();
        }
        const std::string* winner = board.get_winner();
        if (winner != nullptr) {
            winCounts[*winner]++;
        }
    }

    for (const auto& [id, winCount] : winCounts) {
        std::cout << id << ": " << winCount << " / " << ROUND_COUNT << '\n';
    }

    return 0;
}
