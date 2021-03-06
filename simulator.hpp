#ifndef SIMULATOR_INCLUDED
#define SIMULATOR_INCLUDED

#include <functional>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "crow/json.h"

#include "grid.hpp"

namespace Simulator {

    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    
    constexpr const char* direction_to_string(Direction t_direction) {
        switch(t_direction) {
            case Direction::UP:
                return "up";
            case Direction::DOWN:
                return "down";
            case Direction::LEFT:
                return "left";
            case Direction::RIGHT:
                return "right";
        }
    }

    struct Position {
        int x, y;

        friend bool operator==(Position t_p1, Position t_p2);
        friend bool operator!=(Position t_p1, Position t_p2);
    };

    struct PositionHash {
        size_t operator()(const Position& t_pos) const noexcept;
    };

    constexpr Position update_position(Position t_position, Direction t_direction) {
        switch(t_direction) {
            case Direction::UP:
                return Position{t_position.x, t_position.y - 1};
            case Direction::DOWN:
                return Position{t_position.x, t_position.y + 1};
            case Direction::LEFT:
                return Position{t_position.x - 1, t_position.y};
            case Direction::RIGHT:
                return Position{t_position.x + 1, t_position.y};
        }
    }

    class Snake {
    public:
        explicit Snake(const std::vector<Position>& t_body, int t_health=100);
        Snake(Position t_head, unsigned int t_length, int t_health=100);

        // Move without removing the end of the tail
        void move(Direction t_direction);

        // Removes the end of the tail
        void pop_tail();

        void set_health(int t_health);

        [[nodiscard]] Position get_head() const;
        [[nodiscard]] const std::vector<Position>& get_body() const;

        [[nodiscard]] unsigned int get_length() const;
        [[nodiscard]] int get_health() const;

        [[nodiscard]] bool is_alive() const;

        friend bool operator==(const Snake& t_s1, const Snake& t_s2);
        friend struct SnakeHash;
    private:
        std::vector<Position> m_body;
        int m_health;
    };

    struct SnakeHash {
        size_t operator()(const Snake& t_snake) const noexcept;
    };

    struct Ruleset {
        unsigned int w, h, noSnakes, minFood, foodSpawnChance;
        int startingHealth;
        bool spawnFood;

        friend bool operator==(Ruleset t_r1, Ruleset t_r2);
    };

    constexpr Ruleset DEFAULT_RULESET {11, 11, 2, 1, 15, 100, true};

    struct FoodGrid {
        Grid<bool> cells;
        unsigned int count;

        friend bool operator==(const FoodGrid& t_g1, const FoodGrid& t_g2);
        friend struct FoodGridHash;
    };

    struct FoodGridHash {
        size_t operator()(const FoodGrid& t_foodGrid) const noexcept;
    };

    class Board {
    public:
        Board(const std::unordered_map<std::string, Snake>& t_snakes, const FoodGrid& t_food, Ruleset t_ruleset=DEFAULT_RULESET);
        Board(const Board& t_board, Ruleset t_ruleset=DEFAULT_RULESET);

        void update(const std::unordered_map<std::string, Direction>& t_moves);

        [[nodiscard]] Ruleset get_ruleset() const;
        [[nodiscard]] bool is_in_bounds(Position t_position) const;
        [[nodiscard]] bool is_safe_cell(const std::string& t_id, Position t_position) const;

        [[nodiscard]] const std::unordered_map<std::string, Snake>& get_snakes() const;
        [[nodiscard]] const Snake& get_snake(const std::string& t_id) const;
        [[nodiscard]] const FoodGrid& get_food() const;

        // Returns the index of the snake that has won the game
        // If all snakes have been eliminated then m_snakes.size() is returned
        [[nodiscard]] const std::string* get_winner() const;
        [[nodiscard]] bool is_game_over() const;
        [[nodiscard]] bool is_valid_id(const std::string& t_id) const;

        [[nodiscard]] std::string to_string() const;

        friend bool operator==(const Board& t_b1, const Board& t_b2);
        friend struct BoardHash;
    private:
        void feed_snakes();
        void randomly_place_food(unsigned int t_count);
        void spawn_food();
        void eliminate_snakes();

        std::unordered_map<std::string, Snake> m_snakes;
        FoodGrid m_food;

        Ruleset m_ruleset;
    };

    struct BoardHash {
        size_t operator()(const Board& t_ruleset) const noexcept;
    };

}

#endif
