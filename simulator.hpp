#ifndef SIMULATOR_INCLUDED
#define SIMULATOR_INCLUDED

#include <vector>

#include "crow/json.hpp"

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
        unsigned int x, y;
    };

    class Snake {
    public:
        Snake(Position t_head, unsigned int t_length, unsigned int t_health=100);
        Snake(const std::vector<Position>& t_body, unsigned int t_health=100);

        void move(Direction t_direction);

        [[nodiscard]] Position get_head() const;
        [[nodiscard]] unsigned int get_length() const;
        [[nodiscard]] unsigned int get_health() const;

        [[nodiscard]] bool is_alive() const;
    private:
        std::vector<Position> m_body;
        unsigned int m_health;
    };

    struct Ruleset {
        unsigned int w, h, noSnakes, minFood, foodSpawnChance;
    };

    constexpr Ruleset DEFAULT_RULESET {11, 11, 2, 1, 15};

    class Board {
    public:
        Board(Ruleset t_ruleset=DEFAULT_RULESET);
        Board(const std::vector<Snake>& t_snakes, Ruleset t_ruleset=DEFAULT_RULESET);
        Board(const std::vector<Snake>& t_snakes, Grid<bool> t_apples, Ruleset t_ruleset=DEFAULT_RULESET);

        void update(const std::vector<Direction> t_moves);

        [[nodiscard]] Ruleset get_ruleset() const;
        [[nodiscard]] unsigned int get_winner() const;

        [[nodiscard]] std::string to_string() const;

    private:
        const Ruleset m_ruleset;
        std::vector<Snake> m_snakes;
        Grid::Grid<bool> m_apples;
    };

}

#endif