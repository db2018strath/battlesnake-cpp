#ifndef SERVER_LOGIC_INCLUDED
#define SERVER_LOGIC_INCLUDED

#include <string>

#include "crow/json.h"

namespace ServerLogic {

    std::string choose_move(const crow::json::rvalue& t_data);

}

#endif