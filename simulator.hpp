#ifndef SIMULATOR_INCLUDED
#define SIMULATOR_INCLUDED

#include <optional>
#include <unordered_set>
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

    class Snake {
    public:
        Snake(const std::vector<Position>& t_body, int t_health);

        // Move without removing the end of the tail
        void move(Direction t_direction);

        // Removes the end of the tail
        void pop_tail();

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
    };

    constexpr Ruleset DEFAULT_RULESET {11, 11, 2, 1, 15, 100};

    class Board {
    public:
        //Board(Ruleset t_ruleset=DEFAULT_RULESET);
        //Board(const std::vector<Snake>& t_snakes, Ruleset t_ruleset=DEFAULT_RULESET);
        Board(const std::vector<Snake>& t_snakes, Grid<bool> t_apples, Ruleset t_ruleset=DEFAULT_RULESET);

        void update(const std::vector<Direction>& t_moves);

        [[nodiscard]] Ruleset get_ruleset() const;

        // Returns the index of the snake that has won the game
        // If all snakes have been eliminated then m_snakes.size() is returned
        [[nodiscard]] std::optional<unsigned int> get_winner() const;

        [[nodiscard]] std::string to_string() const;

    private:
        std::vector<Snake> m_snakes;
        std::unordered_set<unsigned int> m_eliminatedSnakes;
        Grid<bool> m_apples;
        const Ruleset m_ruleset;
    };

}

#endif
