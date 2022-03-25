#include <catch2/catch.hpp>

#include "../simulator.hpp"

TEST_CASE("Grid<bool> constructor correct") {
    const Grid<bool> grid(10, 10);

    REQUIRE(grid.get_width() == 10);
    REQUIRE(grid.get_height() == 10);
    REQUIRE(grid.size() == 10 * 10);
    for (unsigned int y = 0; y < 10; y++) {
        for (unsigned int x = 0; x < 10; x++) {
            REQUIRE(grid(x, y) == false);
        }
    }
}

TEST_CASE("Grid<bool> operator() correct") {
    Grid<bool> grid(10, 10);

    grid(3, 4) = true;
    REQUIRE(grid(3, 4) == true);

    grid(3, 4) = false;
    REQUIRE(grid(3, 4) == false);
}

TEST_CASE("Grid<bool> operator== correct") {
    Grid<bool> g1(10, 10);
    Grid<bool> g2(10, 10);
    Grid<bool> g3(8, 5);

    Grid<bool> g4(10, 10);
    g4(0, 0) = true;

    Grid<bool> g5(10, 10);
    g5(0, 0) = true;

    REQUIRE(g1 == g1);
    REQUIRE(g1 == g2);
    REQUIRE(!(g1 == g3));
    REQUIRE(!(g1 == g4));
    REQUIRE(!(g1 == g5));

    REQUIRE(g2 == g1);
    REQUIRE(g2 == g2);
    REQUIRE(!(g2 == g3));
    REQUIRE(!(g2 == g4));
    REQUIRE(!(g2 == g5));

    REQUIRE(!(g3 == g1));
    REQUIRE(!(g3 == g2));
    REQUIRE(g3 == g3);
    REQUIRE(!(g3 == g4));
    REQUIRE(!(g3 == g5));

    REQUIRE(!(g4 == g1));
    REQUIRE(!(g4 == g2));
    REQUIRE(!(g4 == g3));
    REQUIRE(g4 == g4);
    REQUIRE(g4 == g5);

    REQUIRE(!(g5 == g1));
    REQUIRE(!(g5 == g2));
    REQUIRE(!(g5 == g3));
    REQUIRE(g5 == g4);
    REQUIRE(g5 == g5);
}


TEST_CASE("Grid<int> constructor correct") {
    const Grid<int> grid(10, 10);

    REQUIRE(grid.get_width() == 10);
    REQUIRE(grid.get_height() == 10);
    REQUIRE(grid.size() == 10 * 10);
    for (unsigned int y = 0; y < 10; y++) {
        for (unsigned int x = 0; x < 10; x++) {
            REQUIRE(grid(x, y) == 0);
        }
    }
}

TEST_CASE("Grid<int> operator() correct") {
    Grid<int> grid(10, 10);

    grid(3, 4) = 1;
    REQUIRE(grid(3, 4) == 1);
    REQUIRE(grid(4, 3) == 0);

    grid(3, 4) = 0;
    REQUIRE(grid(3, 4) == 0);
}

TEST_CASE("Grid<int> operator== correct") {
    Grid<int> g1(10, 10);
    Grid<int> g2(10, 10);
    Grid<int> g3(8, 5);

    Grid<int> g4(10, 10);
    g4(0, 0) = 1;

    Grid<int> g5(10, 10);
    g5(0, 0) = 1;

    REQUIRE(g1 == g1);
    REQUIRE(g1 == g2);
    REQUIRE(!(g1 == g3));
    REQUIRE(!(g1 == g4));
    REQUIRE(!(g1 == g5));

    REQUIRE(g2 == g1);
    REQUIRE(g2 == g2);
    REQUIRE(!(g2 == g3));
    REQUIRE(!(g2 == g4));
    REQUIRE(!(g2 == g5));

    REQUIRE(!(g3 == g1));
    REQUIRE(!(g3 == g2));
    REQUIRE(g3 == g3);
    REQUIRE(!(g3 == g4));
    REQUIRE(!(g3 == g5));

    REQUIRE(!(g4 == g1));
    REQUIRE(!(g4 == g2));
    REQUIRE(!(g4 == g3));
    REQUIRE(g4 == g4);
    REQUIRE(g4 == g5);

    REQUIRE(!(g5 == g1));
    REQUIRE(!(g5 == g2));
    REQUIRE(!(g5 == g3));
    REQUIRE(g5 == g4);
    REQUIRE(g5 == g5);
}
