#include "ai.hpp"
#include "server_logic.hpp"
#include "simulator.hpp"

namespace ServerLogic {
    
    std::string choose_move(const crow::json::rvalue& t_data) {
        const unsigned int w = t_data["board"]["width"].u();
        const unsigned int h = t_data["board"]["height"].u();
        
        std::unordered_map<std::string, Simulator::Snake> snakes;
        auto snakesData = t_data["board"]["snakes"];
        for (const auto& snakeData : snakesData.lo()) {     
            const std::string id = snakeData["id"].s();
            const int health = snakeData["health"].i();

            std::vector<Simulator::Position> body;
            body.reserve(snakeData["length"].u());
            auto bodyData = snakeData["body"];
            auto segments = bodyData.lo();
            for (auto it = segments.rbegin(); it != segments.rend(); ++it) {
                const unsigned int x = (*it)["x"].u();
                const unsigned int y = (*it)["y"].u();
                body.push_back(
                    Simulator::Position{
                        static_cast<int>(x),
                        static_cast<int>(h - 1 - y) // flip y vertically to match online display
                    }
                ); 
            }

            snakes.emplace(id, Simulator::Snake(body, health));
        }

        const unsigned int noSnakes = snakes.size();
        const unsigned int foodSpawnChance = t_data["game"]["ruleset"]["settings"]["foodSpawnChance"].u();
        const unsigned int minFood = t_data["game"]["ruleset"]["settings"]["minimumFood"].u();
                
        const Simulator::Ruleset ruleset{w, h, noSnakes, foodSpawnChance, minFood};

        Grid<bool> food{w, h};
        const unsigned int foodCount = t_data["board"]["food"].size();
        for (const auto& foodData : t_data["board"]["food"]) {
            const unsigned int x = foodData["x"].u();
            const unsigned int y = foodData["y"].u();
            
            food(x, h - 1 - y) = true;
        }
        
         const Simulator::Board board{snakes, Simulator::FoodGrid{food, foodCount}, ruleset};

        const std::string id = t_data["you"]["id"].s();
        
        return Simulator::direction_to_string(AI::seek_food_player(board, id));
    }

}
