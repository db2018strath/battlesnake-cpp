#include <array>
#include <random>

#include "ai.hpp"

namespace AI {

    static std::mt19937 rng(0);

    constexpr std::array<Simulator::Direction, 4> DIRECTIONS_MAP {
        Simulator::Direction::UP,
        Simulator::Direction::DOWN,
        Simulator::Direction::LEFT,
        Simulator::Direction::RIGHT
    };

    Simulator::Direction random_player(const Simulator::Board& t_board, const std::string& t_player) {
        return DIRECTIONS_MAP[rng() % DIRECTIONS_MAP.size()];
    }

    Simulator::Direction avoid_walls_player(const Simulator::Board& t_board, const std::string& t_player) {
        // TODO
        return DIRECTIONS_MAP[0];
    }

}
