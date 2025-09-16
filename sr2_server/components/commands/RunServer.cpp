#include "RunServer.hpp"
#include <crow.h>
#include <crow/query_string.h>

void SR2Server::RunServer::Run(uint16_t port)
{
    crow::SimpleApp app; //define your crow application

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req)
    {
        return crow::response(200, "text/plain", "Hello error");
    });

    CROW_ROUTE(app, "/sample/<uint>").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req, uint32_t index)
    {
        return crow::response(200, "text/plain", "Hello success");
    });

    CROW_ROUTE(app, "/sample_query").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req)
    {
        auto url_params = crow::query_string(req.url_params);
        std::string coder = req.url_params.get("coder") == nullptr ? "" : req.url_params.get("coder");

        std::cout << "url_params: " << url_params << "\n";
        std::cout << "challenge: " << coder << "\n";

        return crow::response(200, "text/html; charset=utf-8", coder);
    });

    //set the port, set the app to run on multiple threads, and run the app
    try
    {
        app.port(port).concurrency(8).run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}