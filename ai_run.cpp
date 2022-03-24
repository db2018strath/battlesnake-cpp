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

    std::cout << board.to_string();
    while (!board.is_game_over()) {
        std::unordered_map<std::string, Simulator::Direction> moveMap;
        for (const auto& [id, s] : snakes) {
            const Simulator::Direction move = AI::mcts_suct_player(board, id, 200);
            std::cout << id << " chose '" << Simulator::direction_to_string(move) << "'\n";
            moveMap[id] = move;
        }
        std::cout << '\n';

        board.update(moveMap);
        std::cout << board.to_string();
    }

    return 0;
}
