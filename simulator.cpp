#include <algorithm>
#include <iostream>
#include <random>

#include "simulator.hpp"

namespace Simulator {

    static std::mt19937 rng(0);

    size_t PositionHash::operator()(const Position& t_pos) const noexcept {
        return std::hash<int>()(t_pos.x) ^ (std::hash<int>()(t_pos.y) << 1);
    }

    bool operator==(Position t_p1, Position t_p2) {
        return ((t_p1.x == t_p2.x) && (t_p1.y == t_p2.y));
    }

    bool operator!=(Position t_p1, Position t_p2) {
        return !(t_p1 == t_p2);
    }

    Snake::Snake(const std::vector<Position>& t_body, int t_health)
        : m_body(t_body)
        , m_health(t_health)
    {
        ;
    }

    Snake::Snake(Position t_head, unsigned int t_length, int t_health) : Snake(std::vector<Position>(t_length, t_head), t_health) {
        ;
    }

    void Snake::move(Direction t_direction) {
        m_body.push_back(update_position(get_head(), t_direction));
        m_health--;
    }

    void Snake::pop_tail() {
        m_body.erase(m_body.begin());
    }

    void Snake::set_health(int t_health) {
        m_health = t_health;
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

    Board::Board(const std::unordered_map<std::string, Snake>& t_snakes, const FoodGrid& t_food, Ruleset t_ruleset)
        : m_snakes(t_snakes)
        , m_food(t_food)
        , m_ruleset(t_ruleset)
    {
        ;
    }

    Board::Board(const Board& t_board, Ruleset t_ruleset) 
        : Board(t_board.get_snakes(), t_board.get_food(), t_ruleset) 
    {
        ;
    }

    void Board::update(const std::unordered_map<std::string, Direction>& t_moves) {
        for (auto& [k, snake] : m_snakes) {
            const auto moveIt = t_moves.find(k);
            if (moveIt != t_moves.end()) {
                snake.move(moveIt->second);
            }
            else {
                snake.move(Simulator::Direction::UP); // TODO: change this
            }
        }

        feed_snakes();
        spawn_food();
        eliminate_snakes();
    }

    Ruleset Board::get_ruleset() const {
        return m_ruleset;
    }

    bool Board::is_in_bounds(Position t_position) const {
        return 
            (t_position.x >= 0 && t_position.x < m_ruleset.w) &&
            (t_position.y >= 0 && t_position.y < m_ruleset.h);
    }

    bool Board::is_safe_cell(const std::string& t_id, Position t_position) const {
        if (!is_in_bounds(t_position)) {
            return false;
        }

        const auto snakeIt = m_snakes.find(t_id);
        if (snakeIt == m_snakes.end()) {
            return false; // TODO: change this
        }

        const Snake& snake = snakeIt->second;

        const auto& body = snake.get_body();
        if (std::find(body.begin(), body.end() - 1, t_position) != body.end() - 1) {
            return false;
        }

        for (auto& [otherId, otherSnake] : m_snakes) {
            if (t_id == otherId) continue;
            
            if (t_position == otherSnake.get_head() && snake.get_length() <= otherSnake.get_length()) {
                return false;
            }  
                
            const auto& otherBody = otherSnake.get_body();
            if (std::find(otherBody.begin(), otherBody.end() - 1, t_position) != otherBody.end() - 1) {
                return false;
            }
        }

        return true;
    }

    const std::unordered_map<std::string, Snake>& Board::get_snakes() const {
        return m_snakes;
    }

    const Snake& Board::get_snake(const std::string& t_id) const {
        const auto snakeIt = m_snakes.find(t_id);
        if (snakeIt != m_snakes.end()) {
            return snakeIt->second;
        }
        else {
            std::cout << t_id << " oops\n";
            return m_snakes.at(t_id);
        }
    }

    const FoodGrid& Board::get_food() const {
        return m_food;
    }

    const std::string* Board::get_winner() const {
        switch (m_snakes.size()) {
            case 1:
                return &m_snakes.begin()->first;
            default:
                return nullptr;
        }
    }

    bool Board::is_game_over() const {
        return (m_snakes.size() <= 1);
    }

    bool Board::is_valid_id(const std::string &t_id) const {
        return m_snakes.count(t_id);
    }

    std::string Board::to_string() const {
        std::string result;

        result += "DIM: " + std::to_string(m_ruleset.w) + 'x' + std::to_string(m_ruleset.h) + '\n';
        for (const auto& [id, snake] : m_snakes) {
            result += "Player " + id + " Health: " + std::to_string(snake.get_health()) + '\n';
        }

        for (int x = 0; x < m_ruleset.w; x++) {
            result += "##";
        }
        result += "###\n";

        for (int y = 0; y < m_ruleset.h; y++) {
            result += "# ";

            for (int x = 0; x < m_ruleset.w; x++) {
                const Position pos{x, y};

                bool snakeFound = false;
                for (auto& [k, snake] : m_snakes) {
                    const auto& body = snake.get_body();
                    if (std::find(body.begin(), body.end(), pos) != body.end()) {
                        if (pos == snake.get_head()) {
                            result += "H";
                        }
                        else {
                            const char c = k.back();
                            result += ('A' <= c && c <= 'Z') ? (c + 0x20) : (c);
                        }
                        snakeFound = true;
                        break;
                    }
                }

                if (!snakeFound) {
                    if (m_food.cells(x, y)) {
                        result += "*";
                    }
                    else {
                        result += " ";
                    }
                }

                result += " ";
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
        std::unordered_set<Position, PositionHash> eatenFood = {};
        for (auto& [k, snake] : m_snakes) {
            const Position head = snake.get_head();
            if (is_in_bounds(head) && m_food.cells(head.x, head.y)) {
                snake.set_health(m_ruleset.startingHealth);
                eatenFood.insert(head);
            }
            else {
                snake.pop_tail();
            }
        }

        for (Position food : eatenFood) {
            m_food.cells(food.x, food.y) = false;
        }
        m_food.count -= eatenFood.size();
    }

    void Board::randomly_place_food(unsigned int t_count) {
        std::vector<Position> freeCells;
        freeCells.reserve(m_food.cells.size());
        
        for (int y = 0; y < m_food.cells.get_height(); y++) {
            for (int x = 0; x < m_food.cells.get_width(); x++) {
                const Position pos = {x, y};
                if (
                    !m_food.cells(x, y) && 
                    std::none_of(
                        m_snakes.begin(),
                        m_snakes.end(),
                        [pos](const auto& keyValue) -> bool {
                            const auto& body = keyValue.second.get_body();
                            return std::find(body.begin(), body.end(), pos) != body.end();
                        }
                    )
                ) {
                    freeCells.push_back(pos);
                }
            }
        }

        std::shuffle(freeCells.begin(), freeCells.end(), rng);

        const unsigned int foodToAdd = std::min(freeCells.size(), static_cast<size_t>(t_count));
        for (unsigned int i = 0; i < foodToAdd; i++) {
            const Position pos = freeCells[i];
            m_food.cells(pos.x, pos.y) = true;
        }
        m_food.count += foodToAdd;

    }

    void Board::spawn_food() {
        if (m_ruleset.spawnFood) {
            if (m_food.count < m_ruleset.minFood) {
                randomly_place_food(m_ruleset.minFood - m_food.count);
            }
            else if (rng() % 100 < m_ruleset.foodSpawnChance) {
                randomly_place_food(1);
            }
        }
    }

    void Board::eliminate_snakes() {
        std::vector<std::string> toBeEliminated;
        for (const auto& [id, snake] : m_snakes) {
            if (!snake.is_alive() || !is_safe_cell(id, snake.get_head())) {
                toBeEliminated.push_back(id);
            }
        }
        for (const auto& id : toBeEliminated) {
            m_snakes.erase(id);
        }
    }

    bool operator==(const Snake& t_s1, const Snake& t_s2) {
        return
            (t_s1.m_health == t_s2.m_health) &&
            (t_s1.m_body == t_s2.m_body);
    }

    size_t SnakeHash::operator()(const Snake& t_snake) const noexcept {
        size_t result = std::hash<int>()(t_snake.m_health);
        for (Position pos : t_snake.m_body) {
            result = (result ^ (PositionHash{}(pos) << 1)) >> 1;
        }
        return result;
    }

    bool operator==(Ruleset t_r1, Ruleset t_r2) {
        return
            (t_r1.w == t_r2.w) &&
            (t_r1.h == t_r2.h) &&
            (t_r1.noSnakes == t_r2.noSnakes) &&
            (t_r1.minFood == t_r2.minFood) &&
            (t_r1.foodSpawnChance == t_r2.foodSpawnChance) &&
            (t_r1.startingHealth == t_r2.startingHealth) &&
            (t_r1.spawnFood == t_r2.spawnFood);
    }

    bool operator==(const FoodGrid& t_g1, const FoodGrid& t_g2) {
        return
            (t_g1.count == t_g2.count) &&
            (t_g1.cells == t_g2.cells);
    }

    size_t FoodGridHash::operator()(const FoodGrid& t_foodGrid) const noexcept {
        size_t result = 0;
        for (unsigned int y = 0; y < t_foodGrid.cells.get_height(); y++) {
            for (unsigned int x = 0; x < t_foodGrid.cells.get_width(); x++) {
                result = (result ^ (std::hash<bool>()(t_foodGrid.cells(x, y)) << 1)) >> 1;
            }
        }

        return result;
    }

    bool operator==(const Board& t_b1, const Board& t_b2) {
        return
            (t_b1.m_ruleset == t_b2.m_ruleset) &&
            (t_b1.m_snakes == t_b2.m_snakes) &&
            (t_b1.m_food == t_b2.m_food);
    }

    size_t BoardHash::operator()(const Board& t_board) const noexcept {
        // return 0;

        ///*
        // ruleset not included in hash as it does not change during the course of a game
        size_t result = FoodGridHash{}(t_board.m_food);
        for (const auto& [id, snake] : t_board.m_snakes) {
            // id not included since it also does not change
            const size_t snakeHash = SnakeHash{}(snake);

            result = (result ^ (snakeHash << 1)) >> 1;
        }

        return result;
        //*/
    }

}
