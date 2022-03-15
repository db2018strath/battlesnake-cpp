#include "server_logic.hpp"
#include "simulator.hpp"

namespace ServerLogic {
    

    std::string choose_move(const crow::json::rvalue& t_data) {
        return Simulator::direction_to_string(Simulator::Direction::LEFT);
    }

}