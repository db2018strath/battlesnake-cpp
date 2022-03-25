#include <catch2/catch.hpp>

#include "../simulator.hpp"

TEST_CASE("Board constructor correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
        {"a", Simulator::Snake({1, 1}, 3)},
        {"b", Simulator::Snake({1, 9}, 3)},
        {"c", Simulator::Snake({9, 1}, 3)},
        {"d", Simulator::Snake({9, 9}, 3)},
    };

    const Simulator::FoodGrid food{Grid<bool>(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h), 0};

    const Simulator::Board board(snakes, food, Simulator::DEFAULT_RULESET);

    REQUIRE(board.get_ruleset() == Simulator::DEFAULT_RULESET);
    REQUIRE(board.is_game_over() == false);
    REQUIRE(board.get_winner() == nullptr);
    REQUIRE(board.get_snakes() == snakes);
    REQUIRE(board.get_food() == food);
}

TEST_CASE("Board is_in_bounds correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
            {"a", Simulator::Snake({1, 1}, 3)},
            {"b", Simulator::Snake({1, 9}, 3)},
            {"c", Simulator::Snake({9, 1}, 3)},
            {"d", Simulator::Snake({9, 9}, 3)},
    };

    const Simulator::FoodGrid food{Grid<bool>(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h), 0};

    const Simulator::Board board(snakes, food, Simulator::DEFAULT_RULESET);

    const unsigned int w = Simulator::DEFAULT_RULESET.w;
    const unsigned int h = Simulator::DEFAULT_RULESET.h;

    REQUIRE(board.is_in_bounds({0, 0}) == true);
    REQUIRE(board.is_in_bounds({w - 1, 0}) == true);
    REQUIRE(board.is_in_bounds({0, h - 1}) == true);
    REQUIRE(board.is_in_bounds({w - 1, h - 1}) == true);

    REQUIRE(board.is_in_bounds({-1, 0}) == false);
    REQUIRE(board.is_in_bounds({0, -1}) == false);
    REQUIRE(board.is_in_bounds({-1, -1}) == false);

    REQUIRE(board.is_in_bounds({w, 0}) == false);
    REQUIRE(board.is_in_bounds({0, h}) == false);
    REQUIRE(board.is_in_bounds({w, h}) == false);

    REQUIRE(board.is_in_bounds({-1, h}) == false);
    REQUIRE(board.is_in_bounds({w, -1}) == false);
}

TEST_CASE("Board is_valid_id correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
            {"a", Simulator::Snake({1, 1}, 3)},
            {"b", Simulator::Snake({1, 9}, 3)},
            {"c", Simulator::Snake({9, 1}, 3)},
            {"d", Simulator::Snake({9, 9}, 3)},
    };

    const Simulator::FoodGrid food{Grid<bool>(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h), 0};

    const Simulator::Board board(snakes, food, Simulator::DEFAULT_RULESET);

    REQUIRE(board.is_valid_id("a") == true);
    REQUIRE(board.is_valid_id("b") == true);
    REQUIRE(board.is_valid_id("c") == true);
    REQUIRE(board.is_valid_id("d") == true);

    REQUIRE(board.is_valid_id("") == false);
    REQUIRE(board.is_valid_id("e") == false);
}

TEST_CASE("Board operator== correct") {
    const std::unordered_map<std::string, Simulator::Snake> s1 {
            {"a", Simulator::Snake({1, 1}, 3)},
            {"b", Simulator::Snake({1, 9}, 3)},
            {"c", Simulator::Snake({9, 1}, 3)},
            {"d", Simulator::Snake({9, 9}, 3)},
    };

    const std::unordered_map<std::string, Simulator::Snake> s2 {
            {"a", Simulator::Snake({1, 1}, 4)},
            {"b", Simulator::Snake({1, 9}, 4)},
            {"c", Simulator::Snake({9, 1}, 4)},
            {"d", Simulator::Snake({9, 9}, 4)},
    };

    const Simulator::Ruleset r1{10, 10, static_cast<unsigned int>(s1.size()), 1, 15, 100, true};
    const Simulator::Ruleset r2{10, 10, static_cast<unsigned int>(s1.size()), 1, 15, 100, false};

    const Simulator::FoodGrid f1{Grid<bool>(10, 10), 0};
    const Simulator::FoodGrid f2{Grid<bool>(9, 9), 0};

    const Simulator::Board b1(s1, f1, r1);
    const Simulator::Board b2(s1, f1, r2);
    const Simulator::Board b3(s1, f2, r1);
    const Simulator::Board b4(s1, f2, r2);
    const Simulator::Board b5(s2, f1, r1);
    const Simulator::Board b6(s2, f1, r2);
    const Simulator::Board b7(s2, f2, r1);
    const Simulator::Board b8(s2, f2, r2);

    const Simulator::Board b9(s1, f1, r1);

    REQUIRE(b1 == b1);
    REQUIRE(!(b1 == b2));
    REQUIRE(!(b1 == b3));
    REQUIRE(!(b1 == b4));
    REQUIRE(!(b1 == b5));
    REQUIRE(!(b1 == b6));
    REQUIRE(!(b1 == b7));
    REQUIRE(!(b1 == b8));
    REQUIRE(b1 == b9);

    REQUIRE(!(b2 == b1));
    REQUIRE(b2 == b2);
    REQUIRE(!(b2 == b3));
    REQUIRE(!(b2 == b4));
    REQUIRE(!(b2 == b5));
    REQUIRE(!(b2 == b6));
    REQUIRE(!(b2 == b7));
    REQUIRE(!(b2 == b8));
    REQUIRE(!(b2 == b9));

    REQUIRE(!(b3 == b1));
    REQUIRE(!(b3 == b2));
    REQUIRE(b3 == b3);
    REQUIRE(!(b3 == b4));
    REQUIRE(!(b3 == b5));
    REQUIRE(!(b3 == b6));
    REQUIRE(!(b3 == b7));
    REQUIRE(!(b3 == b8));
    REQUIRE(!(b3 == b9));

    REQUIRE(!(b4 == b1));
    REQUIRE(!(b4 == b2));
    REQUIRE(!(b4 == b3));
    REQUIRE(b4 == b4);
    REQUIRE(!(b4 == b5));
    REQUIRE(!(b4 == b6));
    REQUIRE(!(b4 == b7));
    REQUIRE(!(b4 == b8));
    REQUIRE(!(b4 == b9));

    REQUIRE(!(b5 == b1));
    REQUIRE(!(b5 == b2));
    REQUIRE(!(b5 == b3));
    REQUIRE(!(b5 == b4));
    REQUIRE(b5 == b5);
    REQUIRE(!(b5 == b6));
    REQUIRE(!(b5 == b7));
    REQUIRE(!(b5 == b8));
    REQUIRE(!(b5 == b9));

    REQUIRE(!(b6 == b1));
    REQUIRE(!(b6 == b2));
    REQUIRE(!(b6 == b3));
    REQUIRE(!(b6 == b4));
    REQUIRE(!(b6 == b5));
    REQUIRE(b6 == b6);
    REQUIRE(!(b6 == b7));
    REQUIRE(!(b6 == b8));
    REQUIRE(!(b6 == b9));

    REQUIRE(!(b7 == b1));
    REQUIRE(!(b7 == b2));
    REQUIRE(!(b7 == b3));
    REQUIRE(!(b7 == b4));
    REQUIRE(!(b7 == b5));
    REQUIRE(!(b7 == b6));
    REQUIRE(b7 == b7);
    REQUIRE(!(b7 == b8));
    REQUIRE(!(b7 == b9));

    REQUIRE(!(b8 == b1));
    REQUIRE(!(b8 == b2));
    REQUIRE(!(b8 == b3));
    REQUIRE(!(b8 == b4));
    REQUIRE(!(b8 == b5));
    REQUIRE(!(b8 == b6));
    REQUIRE(!(b8 == b7));
    REQUIRE(b8 == b8);
    REQUIRE(!(b8 == b9));

    REQUIRE(b9 == b1);
    REQUIRE(!(b9 == b2));
    REQUIRE(!(b9 == b3));
    REQUIRE(!(b9 == b4));
    REQUIRE(!(b9 == b5));
    REQUIRE(!(b9 == b6));
    REQUIRE(!(b9 == b7));
    REQUIRE(!(b9 == b8));
    REQUIRE(b9 == b9);
}

TEST_CASE("Board update out of bounds correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
            {"a", Simulator::Snake({1, 1}, 3)},
            {"b", Simulator::Snake({1, 9}, 3)},
            {"c", Simulator::Snake({9, 1}, 3)},
            {"d", Simulator::Snake({9, 9}, 3)},
    };

    const Simulator::FoodGrid food{Grid<bool>(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h), 0};

    const std::unordered_map<std::string, Simulator::Direction> m1 {
            {"a", Simulator::Direction::UP},
            {"b", Simulator::Direction::UP},
            {"c", Simulator::Direction::UP},
            {"d", Simulator::Direction::UP},
    };

    const std::unordered_map<std::string, Simulator::Direction> m2 {
            {"a", Simulator::Direction::LEFT},
            {"b", Simulator::Direction::LEFT},
            {"c", Simulator::Direction::LEFT},
            {"d", Simulator::Direction::LEFT},
    };

    const Simulator::Board initialBoard(snakes, food, Simulator::DEFAULT_RULESET);

    Simulator::Board board = initialBoard;

    board.update(m1);
    REQUIRE(!(board == initialBoard));
    REQUIRE(board.get_snakes().size() == 4);
    REQUIRE(board.is_game_over() == false);
    REQUIRE(board.get_winner() == nullptr);

    board.update(m1);
    REQUIRE(board.get_snakes().size() == 2);
    REQUIRE(board.is_game_over() == false);
    REQUIRE(board.get_winner() == nullptr);

    board.update(m2);
    board.update(m2);
    REQUIRE(board.get_snakes().size() == 1);
    REQUIRE(board.is_game_over() == true);
    REQUIRE(*board.get_winner() == "d");

}

TEST_CASE("Board update ran out of health correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
            {"a", Simulator::Snake({1, 9}, 3, 1)},
            {"b", Simulator::Snake({2, 9}, 3, 2)},
            {"c", Simulator::Snake({3, 9}, 3, 3)},
            {"d", Simulator::Snake({4, 9}, 3, 4)},
    };

    const Simulator::FoodGrid food{Grid<bool>(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h), 0};

    const std::unordered_map<std::string, Simulator::Direction> m1 {
            {"a", Simulator::Direction::UP},
            {"b", Simulator::Direction::UP},
            {"c", Simulator::Direction::UP},
            {"d", Simulator::Direction::UP},
    };

    Simulator::Board board(snakes, food, Simulator::DEFAULT_RULESET);

    board.update(m1);
    REQUIRE(board.get_snakes().size() == 3);
    REQUIRE(board.is_game_over() == false);
    REQUIRE(board.get_winner() == nullptr);

    board.update(m1);
    REQUIRE(board.get_snakes().size() == 2);
    REQUIRE(board.is_game_over() == false);
    REQUIRE(board.get_winner() == nullptr);

    board.update(m1);
    REQUIRE(board.get_snakes().size() == 1);
    REQUIRE(board.is_game_over() == true);
    REQUIRE(*board.get_winner() == "d");
}

TEST_CASE("Board update head on collision correct") {
    const std::unordered_map<std::string, Simulator::Snake> s1 {
            {"a", Simulator::Snake({0, 0}, 3)},
            {"b", Simulator::Snake({2, 0}, 3)},
    };

    const std::unordered_map<std::string, Simulator::Snake> s2 {
            {"a", Simulator::Snake({0, 0}, 4)},
            {"b", Simulator::Snake({2, 0}, 3)},
    };

    const std::unordered_map<std::string, Simulator::Snake> s3 {
            {"a", Simulator::Snake({0, 0}, 3)},
            {"b", Simulator::Snake({2, 0}, 4)},
    };

    const Simulator::FoodGrid food{Grid<bool>(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h), 0};

    const std::unordered_map<std::string, Simulator::Direction> moves {
            {"a", Simulator::Direction::RIGHT},
            {"b", Simulator::Direction::LEFT},
    };

    Simulator::Board b1(s1, food, Simulator::DEFAULT_RULESET);
    b1.update(moves);
    REQUIRE(b1.get_snakes().size() == 0);
    REQUIRE(b1.is_game_over() == true);
    REQUIRE(b1.get_winner() == nullptr);

    Simulator::Board b2(s2, food, Simulator::DEFAULT_RULESET);
    b2.update(moves);
    REQUIRE(b2.get_snakes().size() == 1);
    REQUIRE(b2.is_game_over() == true);
    REQUIRE(*b2.get_winner() == "a");

    Simulator::Board b3(s3, food, Simulator::DEFAULT_RULESET);
    b3.update(moves);
    REQUIRE(b3.get_snakes().size() == 1);
    REQUIRE(b3.is_game_over() == true);
    REQUIRE(*b3.get_winner() == "b");
}

TEST_CASE("Board update consume food correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
            {"a", Simulator::Snake({1, 9}, 3)},
            {"b", Simulator::Snake({2, 9}, 3)},
    };

    Grid<bool> foodGrid(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h);
    foodGrid(1, 8) = true;

    const Simulator::FoodGrid food{foodGrid, 1};

    const Simulator::Ruleset ruleset{10, 10, static_cast<unsigned int>(snakes.size()), 1, 15, 100, false};

    const std::unordered_map<std::string, Simulator::Direction> moves {
            {"a", Simulator::Direction::UP},
            {"b", Simulator::Direction::UP},
    };

    Simulator::Board board(snakes, food, ruleset);
    board.update(moves);
    REQUIRE(board.get_snake("a").get_health() == 100);
    REQUIRE(board.get_snake("a").get_length() == 4);
    REQUIRE(board.get_snake("b").get_health() == 99);
    REQUIRE(board.get_snake("b").get_length() == 3);
    REQUIRE(board.get_food().cells(1, 8) == false);
    REQUIRE(board.get_food().count == 0);
}

TEST_CASE("Board update spawn food correct") {
    const std::unordered_map<std::string, Simulator::Snake> snakes {
            {"a", Simulator::Snake({1, 9}, 3)},
            {"b", Simulator::Snake({2, 9}, 3)},
    };

    const Grid<bool> foodGrid(Simulator::DEFAULT_RULESET.w, Simulator::DEFAULT_RULESET.h);
    const Simulator::FoodGrid food{foodGrid, 0};

    const Simulator::Ruleset r1{10, 10, static_cast<unsigned int>(snakes.size()), 10, 15, 100, false};
    const Simulator::Ruleset r2{10, 10, static_cast<unsigned int>(snakes.size()), 10, 15, 100, true};

    const std::unordered_map<std::string, Simulator::Direction> moves {
            {"a", Simulator::Direction::UP},
            {"b", Simulator::Direction::UP},
    };

    Simulator::Board b1(snakes, food, r1);
    Simulator::Board b2(snakes, food, r2);
    REQUIRE(b1.get_food().count == 0);
    REQUIRE(b2.get_food().count == 0);

    b1.update(moves);
    b2.update(moves);
    REQUIRE(b1.get_food().count == 0);
    REQUIRE(b2.get_food().count == 10);
}
