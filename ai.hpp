#ifndef AI_INCLUDED
#define AI_INCLUDED

#include <array>

#include "simulator.hpp"

namespace AI {

    Simulator::Direction random_player(const Simulator::Board& t_board, const std::string& t_playerId);
    Simulator::Direction avoid_walls_player(const Simulator::Board& t_board, const std::string& t_playerId);
    Simulator::Direction seek_food_player(const Simulator::Board& t_board, const std::string& t_playerId);

    Simulator::Direction mcts_suct_player(const Simulator::Board& t_board, const std::string& t_playerId, unsigned int t_time=200);
    Simulator::Direction mcts_duct_player(const Simulator::Board& t_board, const std::string& t_playerId, unsigned int t_time=200);

    std::vector<Simulator::Direction> get_safe_moves(const Simulator::Board& t_board, const std::string& t_playerId);

    constexpr std::array<Simulator::Direction, 4> DIRECTIONS_MAP {
        Simulator::Direction::UP,
        Simulator::Direction::DOWN,
        Simulator::Direction::LEFT,
        Simulator::Direction::RIGHT
    };

}

#endif
