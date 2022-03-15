#include <string>

#include "crow.h"

#include "server_logic.hpp"

int main(int argc, char* argv[]) {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return R"({
            "apiversion": "1",
            "author": "db3005",
            "color": "#0000FF",
            "head": "pixel",
            "tail": "pixel"
        })";
    });

    CROW_ROUTE(app, "/start").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        CROW_LOG_INFO << "game start";
        return "ok";
    });

    CROW_ROUTE(app, "/move").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        crow::json::rvalue json = crow::json::load(req.body.c_str(), req.body.length());

        const std::string move = ServerLogic::choose_move(json);
        
        return R"({"move": ")" + move + "\"}";
    });

    CROW_ROUTE(app, "/end").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        CROW_LOG_INFO << "game end";
        return "ok";
    });

    app.port(8080).multithreaded().run();
    
    return 0;
}
