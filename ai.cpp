#include <algorithm>
#include <array>
#include <random>
#include <vector>

#include "ai.hpp"

namespace AI {

    static std::mt19937 rng(0);

    constexpr std::array<Simulator::Direction, 4> DIRECTIONS_MAP {
        Simulator::Direction::UP,
        Simulator::Direction::DOWN,
        Simulator::Direction::LEFT,
        Simulator::Direction::RIGHT
    };

    Simulator::Direction random_player(const Simulator::Board& t_board, const std::string& t_playerId) {
        return DIRECTIONS_MAP[rng() % DIRECTIONS_MAP.size()];
    }

    Simulator::Direction avoid_walls_player(const Simulator::Board& t_board, const std::string& t_playerId) {
        std::vector<Simulator::Direction> possibleMoves(DIRECTIONS_MAP.begin(), DIRECTIONS_MAP.end());

        const auto& playerSnake = t_board.get_snake(t_playerId);
        const auto eraseIt = std::remove_if(
            possibleMoves.begin(),
            possibleMoves.end(),
            [&t_board, &t_playerId, &playerSnake](Simulator::Direction move) -> bool {
                const Simulator::Position nextPos = Simulator::update_position(playerSnake.get_head(), move);
                return !t_board.is_safe_cell(t_playerId, nextPos);
            }
        );
        possibleMoves.erase(eraseIt, possibleMoves.end());
        std::cout << t_playerId << ": ";
        for (const auto move : possibleMoves) {
            std::cout << Simulator::direction_to_string(move) << ' ';
        }
        std::cout << '\n';

        if (!possibleMoves.empty()) {
            return possibleMoves[rng() % possibleMoves.size()];
        }
        else {
            return DIRECTIONS_MAP[0];
        }
    }

}
