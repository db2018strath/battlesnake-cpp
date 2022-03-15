#include <algorithm>
#include <random>

#include "simulator.hpp"

namespace Simulator {

    static std::mt19937 rng(0);

    bool operator==(Position t_p1, Position t_p2) {
        return ((t_p1.x == t_p2.x) && (t_p1.y == t_p2.y));
    }

    Snake::Snake(const std::vector<Position>& t_body, int t_health)
        : m_body(t_body)
        , m_health(t_health)
    {
        ;
    }

    void Snake::move(Direction t_direction) {
        const Position head = get_head();
        switch(t_direction) {
            case Direction::UP:
                m_body.push_back(Position{head.x, head.y - 1});
                break;
            case Direction::DOWN:
                m_body.push_back(Position{head.x, head.y + 1});
                break;
            case Direction::LEFT:
                m_body.push_back(Position{head.x - 1, head.y});
                break;
            case Direction::RIGHT:
                m_body.push_back(Position{head.x + 1, head.y});
                break;
        }
    }

    void Snake::pop_tail() {
        m_body.erase(m_body.begin());
    }

    Position Snake::get_head() const {
        return m_body[get_length() - 1];
    }

    const std::vector<Position>& Snake::get_body() const {
        return m_body;
    }

    unsigned int Snake::get_length() const {
        return m_body.size();
    }

    int Snake::get_health() const {
        return m_health;
    }

    bool Snake::is_alive() const {
        return m_health > 0;
    }

    Board::Board(const std::unordered_map<std::string, Snake>& t_snakes, Grid<bool> t_food, Ruleset t_ruleset)
        : m_snakes(t_snakes)
        , m_food(t_food)
        , m_ruleset(t_ruleset)
    {
        ;
    }

    void Board::update(const std::unordered_map<std::string, Direction>& t_moves) {
        for (auto [k, snake] : m_snakes) {
            snake.move(t_moves[k]);
        }

        feed_snakes();
        spawn_food();
        eliminate_snakes();
    }

    Ruleset Board::get_ruleset() const {
        return m_ruleset;
    }

    std::optional<std::string> Board::get_winner() const {
        switch (m_snakes.size()) {
            case 0:
                return "";
            case 1:
                return m_snakes.begin()->first;
            default:
                return std::nullopt;
        }
    }

    std::string Board::to_string() const {
        std::string result;

        for (int x = 0; x < m_ruleset.w; x++) {
            result += "##";
        }
        result += "###\n";

        for (int y = 0; y < m_ruleset.h; y++) {
            result += "# ";

            for (int x = 0; x < m_ruleset.w; x++) {
                const Position pos{x, y};

                bool snakeFound = false;
                for (auto [k, snake] : m_snakes) {
                    const auto& body = snake.get_body();
                    if (std::find(body.begin(), body.end(), pos) != body.end()) {
                        if (pos == snake.get_head()) {
                            result += "H ";
                        }
                        else {
                            result += k.back() + ' ';
                        }
                        snakeFound = true;
                        break;
                    }
                }

                if (!snakeFound) {
                    if (m_food(x, y)) {
                        result += "* ";
                    }
                    else {
                        result += "  ";
                    }
                }
            }

            result += "#\n";
        }

        for (unsigned int x = 0; x < m_ruleset.w; x++) {
            result += "##";
        }
        result += "###\n";

        return result;
    }


    void Board::feed_snakes() {
        std::unordered_set<Position> eatenFood;
        
        for (auto [k, snake] : m_snakes) {
            const Position head = snake.get_head();
            if (m_food(head.x, head.y)) {
                snake.set_health(m_ruleset.startingHealth);
                eatenFood.add(head);
            }
            else {
                snake.pop_tail();
            }
        }

        for (Position food : eatenFood) {
            m_food(food.x, food.y) = false;
        }
    }

    void Board::randomly_place_food(unsigned int t_count) {
        // TODO
    }

    void Board::spawn_food() {
        /*
        if (m_ruleset.spawnFood) {
            if (m_foodCount < m_ruleset.minFood) {
                randomly_place_food(m_ruleset.minFood - m_foodCount);
            }
            else if (rng() % 100 < m_ruleset.foodSpawnChance) {
                randomly_place_food(1);
            }
        }
        */
    }

    void Board::eliminate_snakes() {
        std::unordered_set<std::string> toBeEliminated;

        for (auto [key , snake] : m_snakes) {
            const Position head = snake.get_head();
            if (!is_in_bounds(head) || !snake.is_alive()) {
                toBeEliminated.add(key);
                continue;
            }

            const auto& body = snake.get_body();
            if (std::find(std::next(body.begin()), body.end(), head) != body.end()) {
                toBeEliminated.add(key);
                continue;
            }

            for (auto [otherKey, otherSnake] : m_snakes) {
                if (otherKey == key) continue;
                
                if (head == otherSnake.get_head()) {
                    if (snake.get_length() <= otherSnake.length()) {
                        toBeEliminated.add(key);
                    }
                    break;
                }  
                    
                const auto& otherBody = otherSnake.get_body();
                if (std::find(otherBody.begin(), otherBody.end() - 1, head) != otherBody.end()) {
                    toBeEliminated.add(key);
                    break;
                }
            }
        }

        for (auto k : toBeEliminated) {
            m_snakes.erase(k);
        }
    }

}
