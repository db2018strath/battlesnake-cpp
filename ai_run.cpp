#include <iostream>

#include "ai.hpp"
#include "simulator.hpp"

int main(int argc, char* argv[]) {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
        {"a", Simulator::Snake(Simulator::Position{1, 1}, 3, 100)},
        {"b", Simulator::Snake(Simulator::Position{9, 9}, 3, 100)},
    };

    const Simulator::FoodGrid food = {Grid<bool>(11, 11), 0};
    Simulator::Board board{snakes, food};

    std::cout << board.to_string() << '\n';
    while (board.get_winner() == std::nullopt) {
        std::unordered_map<std::string, Simulator::Direction> moveMap;
        for (const auto& [id, s] : snakes) {
            moveMap[id] = AI::random_player(board, id);
        }

        board.update(moveMap);
        std::cout << board.to_string() << '\n';
    }

    return 0;
}
