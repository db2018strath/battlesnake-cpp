#include <algorithm>

#include "simulator.hpp"

namespace Simulator {

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


    Board::Board(const std::vector<Snake>& t_snakes, Grid<bool> t_apples, Ruleset t_ruleset)
        : m_snakes(t_snakes)
        , m_eliminatedSnakes({})
        , m_apples(t_apples)
        , m_ruleset(t_ruleset)
    {
        ;
    }

    void Board::update(const std::vector<Direction>& t_moves) {
        // TODO
    }

    Ruleset Board::get_ruleset() const {
        return m_ruleset;
    }

    std::optional<unsigned int> Board::get_winner() const {
        std::optional<unsigned int> winner = std::nullopt;
        for (unsigned int i = 0; i < m_snakes.size(); i++) {
            if (!m_eliminatedSnakes.count(i)) {
                if (winner == std::nullopt) {
                    winner = i;
                }
                else {
                    winner = m_snakes.size();
                    break;
                }
            }
        }

        return winner;
    }

    std::string Board::to_string() const {
        std::string result;

        for (unsigned int x = 0; x < m_ruleset.w; x++) {
            result += "###";
        }

        for (unsigned int y = 0; y < m_ruleset.h; y++) {
            result += "# ";

            for (unsigned int x = 0; x < m_ruleset.w; x++) {
                const Position pos{x, y};
                for (unsigned int i = 0; i < m_snakes.size(); i++) {
                    const auto& body = m_snakes[i].get_body();
                    if (std::find(body.begin(), body.end(), pos) != body.end()) {
                        result += std::to_string(i) + ' ';
                    }
                    else if (m_apples(x, y)) {
                        result += "* ";
                    }
                    else {
                        result += "  ";
                    }
                }
            }

            result += '#';
        }

        for (unsigned int x = 0; x < m_ruleset.w; x++) {
            result += "###";
        }

        return result;
    }

}
