#ifndef SIMULATOR_INCLUDED
#define SIMULATOR_INCLUDED

#include <functional>
#include <optional>
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
    };

    struct PositionHash {
        size_t operator()(const Position& t_pos) const noexcept;
    };

    class Snake {
    public:
        Snake(const std::vector<Position>& t_body, int t_health);

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
    private:
        std::vector<Position> m_body;
        int m_health;
    };

    struct Ruleset {
        unsigned int w, h, noSnakes, minFood, foodSpawnChance;
        int startingHealth;
        bool spawnFood;
    };

    constexpr Ruleset DEFAULT_RULESET {11, 11, 2, 1, 15, 100, true};

    struct FoodGrid {
        Grid<bool> cells;
        unsigned int count;
    };

    class Board {
    public:
        //Board(Ruleset t_ruleset=DEFAULT_RULESET);
        //Board(const std::vector<Snake>& t_snakes, Ruleset t_ruleset=DEFAULT_RULESET);
        Board(const std::unordered_map<std::string, Snake>& t_snakes, const FoodGrid& t_food, Ruleset t_ruleset=DEFAULT_RULESET);

        void update(const std::unordered_map<std::string, Direction>& t_moves);

        [[nodiscard]] Ruleset get_ruleset() const;

        // Returns the index of the snake that has won the game
        // If all snakes have been eliminated then m_snakes.size() is returned
        [[nodiscard]] std::optional<std::string> get_winner() const;

        [[nodiscard]] std::string to_string() const;

    private:
        void feed_snakes();
        void randomly_place_food(unsigned int t_count);
        void spawn_food();
        void eliminate_snakes();

        [[nodiscard]] bool is_in_bounds(Position t_position) const;

        std::unordered_map<std::string, Snake> m_snakes;
        FoodGrid m_food;

        const Ruleset m_ruleset;
    };

}

#endif
