#ifndef AI_INCLUDED
#define AI_INCLUDED

#include "simulator.hpp"

namespace AI {

    Simulator::Direction random_player(const Simulator::Board& t_board, const std::string& t_playerId);
    Simulator::Direction avoid_walls_player(const Simulator::Board& t_board, const std::string& t_playerId);
    Simulator::Direction seek_food_player(const Simulator::Board& t_board, const std::string& t_playerId);

    Simulator::Direction mcts_suct_player(const Simulator::Board& t_board, const std::string& t_playerId);
    Simulator::Direction mcts_duct_player(const Simulator::Board& t_board, const std::string& t_playerId);

}

#endif
