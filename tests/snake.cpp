#include <unordered_map>

#include <catch2/catch.hpp>

#include "../simulator.hpp"

TEST_CASE("Snake constructors correct") {
    const Simulator::Snake s1({{1, 1}, {1, 2}, {2, 2}}, 50);
    REQUIRE(s1.get_length() == 3);
    REQUIRE(s1.get_health() == 50);
    REQUIRE(s1.get_head() == Simulator::Position{2, 2});

    const Simulator::Snake s2(Simulator::Position{1, 1}, 5, 50);
    REQUIRE(s2.get_length() == 5);
    REQUIRE(s2.get_health() == 50);
    REQUIRE(s2.get_head() == Simulator::Position{1, 1});
    for (const Simulator::Position& position : s2.get_body()) {
        REQUIRE(position == Simulator::Position{1, 1});
    }
}

TEST_CASE("Snake is_alive correct") {
    const Simulator::Snake s1({1, 1}, 3, 100);
    REQUIRE(s1.is_alive() == true);

    const Simulator::Snake s2({1, 1}, 3, 0);
    REQUIRE(s2.is_alive() == false);

    const Simulator::Snake s3({1, 1}, 3, -100);
    REQUIRE(s3.is_alive() == false);
}

TEST_CASE("Snake pop_tail correct") {
    Simulator::Snake snake({{1, 1}, {2, 1}, {3, 1}}, 100);

    snake.pop_tail();
    REQUIRE(snake.get_length() == 2);
    REQUIRE(snake.get_head() == Simulator::Position{3, 1});

    snake.pop_tail();
    REQUIRE(snake.get_length() == 1);
    REQUIRE(snake.get_head() == Simulator::Position{3, 1});

    snake.pop_tail();
    REQUIRE(snake.get_length() == 1);
    REQUIRE(snake.get_head() == Simulator::Position{3, 1});
}

TEST_CASE("Snake set_health correct") {
    Simulator::Snake snake({1, 1}, 3, 100);

    snake.set_health(50);
    REQUIRE(snake.get_health() == 50);
    REQUIRE(snake.is_alive() == true);

    snake.set_health(-100);
    REQUIRE(snake.get_health() == -100);
    REQUIRE(snake.is_alive() == false);

    snake.set_health(50);
    REQUIRE(snake.get_health() == 50);
    REQUIRE(snake.is_alive() == true);
}

TEST_CASE("Snake move correct") {
    Simulator::Snake snake({1, 1}, 3, 100);
    snake.move(Simulator::Direction::UP);
    REQUIRE(snake.get_head() == Simulator::Position{1, 0});
    REQUIRE(snake.get_length() == 4);
    REQUIRE(snake.get_health() == 99);

    snake.move(Simulator::Direction::RIGHT);
    REQUIRE(snake.get_head() == Simulator::Position{2, 0});
    REQUIRE(snake.get_length() == 5);
    REQUIRE(snake.get_health() == 98);

    snake.move(Simulator::Direction::DOWN);
    REQUIRE(snake.get_head() == Simulator::Position{2, 1});
    REQUIRE(snake.get_length() == 6);
    REQUIRE(snake.get_health() == 97);

    snake.move(Simulator::Direction::LEFT);
    REQUIRE(snake.get_head() == Simulator::Position{1, 1});
    REQUIRE(snake.get_length() == 7);
    REQUIRE(snake.get_health() == 96);
}

