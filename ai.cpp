#include <algorithm>
#include <array>
#include <limits>
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

    std::vector<Simulator::Direction> get_safe_moves(const Simulator::Board& t_board, const std::string& t_playerId);
    unsigned int grid_distance(Simulator::Position t_p1, Simulator::Position t_p2);

    Simulator::Direction random_player(const Simulator::Board& t_board, const std::string& t_playerId) {
        return DIRECTIONS_MAP[rng() % DIRECTIONS_MAP.size()];
    }

    Simulator::Direction avoid_walls_player(const Simulator::Board& t_board, const std::string& t_playerId) {
        const std::vector<Simulator::Direction> possibleMoves = get_safe_moves(t_board, t_playerId);
        
        //std::cout << t_playerId << ": ";
        //for (const auto move : possibleMoves) {
        //    std::cout << Simulator::direction_to_string(move) << ' ';
        //}
        //std::cout << '\n';

        if (!possibleMoves.empty()) {
            return possibleMoves[rng() % possibleMoves.size()];
        }
        else {
            return DIRECTIONS_MAP[0];
        }
    }

    Simulator::Direction seek_food_player(const Simulator::Board& t_board, const std::string& t_playerId) {
        const std::vector<Simulator::Direction> possibleMoves = get_safe_moves(t_board, t_playerId);

        const Simulator::Snake& snake = t_board.get_snake(t_playerId);
        const Simulator::FoodGrid& food = t_board.get_food();
        const Simulator::Position head = snake.get_head();
        const Simulator::Ruleset ruleset = t_board.get_ruleset();
        
        Simulator::Position closestFood = head;
        unsigned int closestFoodDistance = std::numeric_limits<unsigned int>::max();
        for (int y = 0; y < ruleset.h; y++) {
            for (int x = 0; x < ruleset.w; x++) {
                const Simulator::Position pos = Simulator::Position{x, y};
                const unsigned int distance = grid_distance(pos, head);
                
                if (food.cells(x, y) &&  distance < closestFoodDistance) {
                    closestFood = pos;
                    closestFoodDistance = distance;
                }
            }
        }

        std::vector<Simulator::Direction> seekingMoves = possibleMoves;
        const auto eraseIt = std::remove_if(
            seekingMoves.begin(),
            seekingMoves.end(),
            [closestFood, closestFoodDistance, head](Simulator::Direction move) {
                const Simulator::Position nextPos = Simulator::update_position(head, move);
                return grid_distance(nextPos, closestFood) >= closestFoodDistance;
            }
        );
        seekingMoves.erase(eraseIt, seekingMoves.end());

        if (!seekingMoves.empty()) {
            return seekingMoves[rng() % seekingMoves.size()];
        }
        else if (!possibleMoves.empty()) {
            return possibleMoves[rng() % possibleMoves.size()];
        }
        else {
            return DIRECTIONS_MAP[0];
        }
        
    }

    std::vector<Simulator::Direction> get_safe_moves(const Simulator::Board& t_board, const std::string& t_playerId) {
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
        
        return possibleMoves;
    }

    unsigned int grid_distance(Simulator::Position t_p1, Simulator::Position t_p2) {
        return std::abs(t_p1.x - t_p2.x) + std::abs(t_p1.y - t_p2.y);
    }

}
