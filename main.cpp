#include "crow.h"

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

    app.port(8080).multithreaded().run();
    
    return 0;
}