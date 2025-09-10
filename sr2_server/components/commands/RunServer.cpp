#include "RunServer.hpp"
#include <crow.h>

void SR2Server::RunServer::Run(uint16_t port)
{
    crow::SimpleApp app; //define your crow application

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req)
    {
        return crow::response(400, "text/plain", "Hello error");
    });

    CROW_ROUTE(app, "/sample/<uint>").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req, uint32_t index)
    {
        return crow::response(200, "text/plain", "Hello success");
    });

    //set the port, set the app to run on multiple threads, and run the app
    try
    {
        app.port(port).multithreaded().run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}